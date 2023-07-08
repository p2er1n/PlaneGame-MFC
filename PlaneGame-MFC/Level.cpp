#include "Level.h"
#include "MainWnd.h"

Level::Level() :
	m_nCurEnemyCnt(0), m_nEnemyCnt(5), m_nHard(1), m_nScore(0),m_ObjList(1),
	m_ObjList_buffer(1)
{

}

Level::~Level()
{
	//删除关卡buffer对象
	for (int i = 0; i < m_ObjList_buffer.size(); ++i) {
		for (auto it = m_ObjList_buffer[i].begin(); it != m_ObjList_buffer[i].end();) {
			delete* it;
			it = m_ObjList_buffer[i].erase(it);
		}
	}
	//删除关卡对象
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

	//生成敌人
	if (m_nCurEnemyCnt < m_nEnemyCnt) {
		m_ObjList[enemy].push_back(new NormalEnemy(rand()%GAME_WIDTH, rand()%10));
		m_nCurEnemyCnt++;
	}

	//更新敌人状态
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end(); ++it) {
			((Enemy*)(*it))->Update();
		}
	}

	//检测玩家攻击
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

	//更新敌人存活
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
	//打包规则
	//<标识（关卡还是战机）>,<类种类数量m>,...(<类名>,<数量n>,...<n个类实例的包>)

	std::string packet;

	//标识
	packet += "Level";
	packet += ",";

	//类种类数量
	int m = m_ObjList.size();
	packet += std::to_string(m);
	packet += ",";

	//每种类
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

	std::lock_guard<std::mutex> lock(m_ObjList_buffer_lock);//上锁！！
	//保证删除m_ObjList_buffer的时候，没其他线程访问他，
	//否则会获得错误的迭代器


	//解包规则
    //<标识（关卡还是战机）>,<类种类数量m>,...(<类名>,<数量n>,...<n个类实例的包>)

	//包头已经被解析了之后扔掉了，直接从类数量开始

	//先保存到buffer

	//清理掉之前的ObjList buffer
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
		int cnt = atoi(std::string(packet.begin(),it).c_str());//类实例包的数量
		packet = std::string(it + 1, packet.end());
		
		auto itEnemy = m_ObjList_buffer[enemy].begin();
		std::string p;
		if (className == "NormalEnemy") {
			for (int j = 0; j < cnt; ++j) {
				// 每个类实例包取出来
				it = find(packet.begin(), packet.end(), ',');
				p = std::string(packet.begin(), it);
				if (it == packet.end()) {
					//取到最后一个，后面没了','所以返回尾迭代器
					packet.clear();
				}
				packet.erase(packet.begin(),it + 1);

				//判断本地有没有实例化，没有就实例化一个再更新，有就直接更新 
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
	std::lock_guard<std::mutex> lock(m_ObjList_buffer_lock);//锁上
	//保证访问m_ObjList_buffer_lock的时候迭代器有效
 
	//把ObjList buffer的内容同步到ObjList

	//删掉ObjList原来的内容
	
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end();) {
			delete* it;
			it = m_ObjList[i].erase(it);
		}
	}

	//同步实例对象信息
	for (int i = 0; i < m_ObjList.size(); ++i) {
		for (auto it = m_ObjList_buffer[i].begin(); it != m_ObjList_buffer[i].end(); ++it) {
			std::string className = (*it)->mGetClassName();
			if (className == "NormalEnemy") {
				CPoint ptPos = (*it)->GetPoint();
				m_ObjList[i].push_back(new NormalEnemy(*static_cast<NormalEnemy*>(*it)));//先只同步坐标<FIXME>
			}
		}
	}
	
}


