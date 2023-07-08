#include "NormalEnemy.h"

const static int s_width = 35;
const static int s_height = 35;
const static int s_speed = 5;
const static int s_shootSpeed = 50;
const static UINT s_health = 10;
CImageList NormalEnemy::m_ImageList;
BOOL NormalEnemy::m_fLoad = FALSE;

NormalEnemy::NormalEnemy(int x, int y) :
	Enemy(x, y, s_speed, IDB_NORMALENEMY, s_width,s_height,s_shootSpeed,s_health)
{
	if (!m_fLoad) {
		Load(m_ImageList);
		m_fLoad = TRUE;
	}
}

CImageList& NormalEnemy::GetImageList()
{
	return m_ImageList;
}

void NormalEnemy::LoadBullet()
{
	CPoint cPos = GetPoint();
	m_Bullets.push_back(new NormalEnemyBullet(cPos.x + m_iWidth/2 - 5,cPos.y + m_iHeight));
}

std::string NormalEnemy::Pack() {
	//NormalEnemy 打包规则：x|y|n子弹数目(NormalEnemyBullet)|x1|y1|...|xn|yn

	std::string packet;

	CPoint ptPos = GetPoint();

	packet += std::to_string(ptPos.x);
	packet += "|";

	packet += std::to_string(ptPos.y);
	packet += "|";

	size_t bulletCnt = m_Bullets.size();
	packet += std::to_string(bulletCnt);
	packet += "|";

	for (auto it = m_Bullets.begin();it != m_Bullets.end(); ++it) {
		if (it != m_Bullets.begin())
			packet += "|";
		CPoint ptPos = (*it)->GetPoint();
		packet += std::to_string(ptPos.x);
		packet += "|";
		packet += std::to_string(ptPos.y);
	}

	return packet;
}

BOOL NormalEnemy::Unpack(std::string& packet) {
	//NormalEnemy 解包规则：x|y|n子弹数目(NormalEnemyBullet)|x1|y1|...|xn|yn
	CPoint ptPos;
	//x
	auto it = find(packet.begin(), packet.end(), '|');
	ptPos.x = atoi(std::string(packet.begin(), it).c_str());
	packet = std::string(it + 1, packet.end());
	//y
	it = find(packet.begin(), packet.end(), '|');
	ptPos.y = atoi(std::string(packet.begin(), it).c_str());
	packet = std::string(it + 1, packet.end());

	//设置坐标
	SetPos(ptPos);

	//子弹
	it = find(packet.begin(), packet.end(), '|');
	int n = atoi(std::string(packet.begin(), it).c_str());
	packet = std::string(it + 1, packet.end());

	auto itBullet = m_Bullets.begin();
	for (int i = 0; i < n; ++i) {
		//x
		CPoint ptPos;
		it = find(packet.begin(), packet.end(), '|');
		ptPos.x = atoi(std::string(packet.begin(), it).c_str());
		packet = std::string(it + 1, packet.end());
		//y
		if (i != n - 1) {
			it = find(packet.begin(), packet.end(), '|');
			ptPos.y = atoi(std::string(packet.begin(), it).c_str());
			packet = std::string(it + 1, packet.end());
		}
		else {
			ptPos.y = atoi(packet.c_str());
		}

		//更新子弹
		if (itBullet != m_Bullets.end()) {
			(*itBullet)->SetPos(ptPos);
			itBullet++;
		}
		else {
			itBullet = m_Bullets.insert(itBullet, new NormalEnemyBullet(ptPos.x, ptPos.y));
		}

	}

	for (; itBullet != m_Bullets.end();) {
		delete* itBullet;
		itBullet = m_Bullets.erase(itBullet);
	}

	return TRUE;
}

std::string NormalEnemy::mGetClassName() {
	return m_sClassName;
}

NormalEnemy::NormalEnemy(const NormalEnemy& e) :
	NormalEnemy(e.m_ptPos.x,e.m_ptPos.y)
{
	//clear
	for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
		delete* it;
		it = m_Bullets.erase(it);
	}

	//copy
	for (auto it = e.m_Bullets.begin(); it != e.m_Bullets.end(); ++it) {
		CPoint ptPos = (*it)->GetPoint();
		m_Bullets.push_back(new NormalEnemyBullet(ptPos.x,ptPos.y));
	}
}