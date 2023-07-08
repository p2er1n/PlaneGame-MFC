#include "Level.h"
#include "MainWnd.h"

Level::Level() :
	m_nCurEnemyCnt(0), m_nEnemyCnt(5), m_nHard(1), m_nScore(0),m_ObjList(1),
	m_ObjList_buffer(1)
{

}

Level::~Level()
{
	//ɾ���ؿ�buffer����
	for (int i = 0; i < m_ObjList_buffer.size(); ++i) {
		for (auto it = m_ObjList_buffer[i].begin(); it != m_ObjList_buffer[i].end();) {
			delete* it;
			it = m_ObjList_buffer[i].erase(it);
		}
	}
	//ɾ���ؿ�����
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end();) {
			delete* it;
			it = m_ObjList[i].erase(it);
		}
	}
}

void Level::Update(Plane* pPlane)
{

	//clean
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end();) {
			CPoint ptPos = (*it)->GetPoint();
			if (ptPos.x <0 || ptPos.x > GAME_WIDTH || ptPos.y < 0 || ptPos.y > GAME_HEIGHT) {
				delete* it;
				it = m_ObjList[i].erase(it);
				if (i == enemy)
					m_nCurEnemyCnt--;
			}
			else
				it++;
		}
	}

	//���ɵ���
	if (m_nCurEnemyCnt < m_nEnemyCnt) {
		m_ObjList[enemy].push_back(new NormalEnemy(rand()%GAME_WIDTH, rand()%10));
		m_nCurEnemyCnt++;
	}

	//���µ���״̬
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end(); ++it) {
			((Enemy*)(*it))->Update();
		}
	}

	//�����ҹ���
	auto& PlayerBullets = pPlane->GetBullets();
	for (auto itPlayerBullet = PlayerBullets.begin(); itPlayerBullet != PlayerBullets.end();) {
		bool fAttacked = false;
		for (auto itTarget = m_ObjList[enemy].begin(); itTarget != m_ObjList[enemy].end();++itTarget) {
			if ((*itPlayerBullet)->isCollision(*itTarget)) {
				((Enemy*)(*itTarget))->DescreaseHealth((*itPlayerBullet)->GetAttack());
				fAttacked = true;
				break;
			}
   		}

		if (fAttacked) {
			delete* itPlayerBullet;
			itPlayerBullet = PlayerBullets.erase(itPlayerBullet);

		}
		else {
			itPlayerBullet++;
		}
	}

	//���µ��˴��
	for (auto it = m_ObjList[enemy].begin(); it != m_ObjList[enemy].end();) {
		if (((Enemy*)(*it))->GetHealth() <= 0) {

			delete* it;
			it = m_ObjList[enemy].erase(it);

			m_nCurEnemyCnt--;
		}
		else
			it++;
	}
}

void Level::Draw(CDC* pDC)
{
	//Draw background
	pDC->FillSolidRect(CRect(0,0,GAME_WIDTH,GAME_HEIGHT),RGB(156,211,251));

	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end(); ++it) {
			(*it)->Draw(pDC, (*it)->GetImageList());
		}
	}
}

std::string Level::Pack()
{
	//�������
	//<��ʶ���ؿ�����ս����>,<����������m>,...(<����>,<����n>,...<n����ʵ���İ�>)

	std::string packet;

	//��ʶ
	packet += "Level";
	packet += ",";

	//����������
	int m = m_ObjList.size();
	packet += std::to_string(m);
	packet += ",";

	//ÿ����
	for (int i = 0; i < m; ++i) {
		if (i != 0)
			packet += ",";

		auto it = m_ObjList[i].begin();
		std::string className = (*it)->mGetClassName();
		packet += className;
		packet += ",";

		int n = m_ObjList[i].size();
		packet += std::to_string(n);
		packet += ",";

		for (; it != m_ObjList[i].end(); ++it) {
			if (!(it == m_ObjList[i].begin()))
				packet += ",";
			std::string p = (*it)->Pack();
			packet += p;
		}
	}
	return packet;
}

BOOL Level::Unpack(std::string& packet)
{

	std::lock_guard<std::mutex> lock(m_ObjList_buffer_lock);//��������
	//��֤ɾ��m_ObjList_buffer��ʱ��û�����̷߳�������
	//������ô���ĵ�����


	//�������
    //<��ʶ���ؿ�����ս����>,<����������m>,...(<����>,<����n>,...<n����ʵ���İ�>)

	//��ͷ�Ѿ���������֮���ӵ��ˣ�ֱ�Ӵ���������ʼ

	//�ȱ��浽buffer

	//�����֮ǰ��ObjList buffer
	for (int i = 0; i < m_ObjList_buffer.size(); ++i) {
		for (auto it = m_ObjList_buffer[i].begin(); it != m_ObjList_buffer[i].end();) {
			delete* it;
			it = m_ObjList_buffer[i].erase(it);
		}
	}

	auto it = find(packet.begin(),packet.end(),',');
	int n = atoi(std::string(packet.begin(),it).c_str());
	packet = std::string(it+1,packet.end());

	for (int i = 0; i < n; ++i) {
		it = find(packet.begin(), packet.end(), ',');
		std::string className(packet.begin(),it);
		packet = std::string(it + 1,packet.end());

		it = find(packet.begin(),packet.end(),',');
		int cnt = atoi(std::string(packet.begin(),it).c_str());//��ʵ����������
		packet = std::string(it + 1, packet.end());
		
		auto itEnemy = m_ObjList_buffer[enemy].begin();
		std::string p;
		if (className == "NormalEnemy") {
			for (int j = 0; j < cnt; ++j) {
				// ÿ����ʵ����ȡ����
				it = find(packet.begin(), packet.end(), ',');
				p = std::string(packet.begin(), it);
				if (it == packet.end()) {
					//ȡ�����һ��������û��','���Է���β������
					packet.clear();
				}
				packet.erase(packet.begin(),it + 1);

				//�жϱ�����û��ʵ������û�о�ʵ����һ���ٸ��£��о�ֱ�Ӹ��� 
				if (itEnemy != m_ObjList_buffer[enemy].end()) {
					static_cast<NormalEnemy*>(*itEnemy)->Unpack(p);
				}
				else {
					itEnemy = m_ObjList_buffer[enemy].insert(itEnemy, new NormalEnemy(0, 0));
					static_cast<NormalEnemy*>(*itEnemy)->Unpack(p);
				}
				itEnemy++;
			}
		}
	}

	return TRUE;
}

void Level::Clear()
{
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end();) {
			delete* it;
			it = m_ObjList[i].erase(it);
		}
	}

}

void Level::Sync()
{
	std::lock_guard<std::mutex> lock(m_ObjList_buffer_lock);//����
	//��֤����m_ObjList_buffer_lock��ʱ���������Ч
 
	//��ObjList buffer������ͬ����ObjList

	//ɾ��ObjListԭ��������
	
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end();) {
			delete* it;
			it = m_ObjList[i].erase(it);
		}
	}

	//ͬ��ʵ��������Ϣ
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList_buffer[i].begin(); it != m_ObjList_buffer[i].end(); ++it) {
			std::string className = (*it)->mGetClassName();
			if (className == "NormalEnemy") {
				CPoint ptPos = (*it)->GetPoint();
				m_ObjList[i].push_back(new NormalEnemy(*static_cast<NormalEnemy*>(*it)));//��ֻͬ������<FIXME>
			}
		}
	}
	
}


