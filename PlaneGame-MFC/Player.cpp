#include "Player.h"
#include "MainWnd.h"
#include "NormalPlayerBullet.h"

CImageList Player::m_ImageList;
BOOL Player::m_fLoad = FALSE;
const static int s_DefaultShootSpeed = 100;
const static int s_health = 100;

Player::Player(int x, int y, int speed) :
	Plane(x,y,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(0,0,0),1,IDB_PLAYER,speed,
		s_DefaultShootSpeed, s_health)
{
	if (!m_fLoad) {
		Load(m_ImageList);
		m_fLoad = TRUE;
	}
}

CImageList& Player::GetImageList()
{
	return m_ImageList;
}

BOOL Player::Move(int cx, int cy)
{
	if (m_ptPos.x + cx + m_iWidth > GAME_WIDTH || m_ptPos.x + cx < 0)
		cx = 0;
	if (m_ptPos.y + cy + m_iHeight > GAME_HEIGHT || m_ptPos.y + cy < 0)
		cy = 0;

	return Plane::Move(cx,cy);
}

void Player::Update()
{

	Plane::Update();

	int nDir[2] = { 0 };//Ver Hor
	if (GetKeyState('W') & 0x8000) {
		nDir[0] = -1;
	}
	if (GetKeyState('S') & 0x8000) {
		nDir[0] = 1;
	}
	if (GetKeyState('A') & 0x8000) {
		nDir[1] = -1;
	}
	if (GetKeyState('D') & 0x8000) {
		nDir[1] = 1;
	}

	Move(nDir[1]*m_nSpeed,nDir[0]*m_nSpeed);
}

void Player::LoadBullet()
{
	CPoint cPos = GetPoint();
	m_Bullets.push_back(new NormalPlayerBullet(cPos.x + m_iWidth/2 - 5, cPos.y));//等待加入战机子弹，先用敌机的
}

std::string Player::Pack()
{
	//打包规则：
	//x|y|n子弹数目(NormalPlayerBullet)|x1|y1|...|xn|yn
	std::string packet;
	CPoint ptPos = GetPoint();

	packet += std::to_string(ptPos.x);
	packet += "|";

	packet += std::to_string(ptPos.y);
	packet += "|";

	int bulletCnt = m_Bullets.size();
	packet += std::to_string(bulletCnt);
	packet += "|";

	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ++it) {
		if (it != m_Bullets.begin())
			packet += "|";
		CPoint ptPos = (*it)->GetPoint();
		packet += std::to_string(ptPos.x);
		packet += "|";
		packet += std::to_string(ptPos.y);
	}

	return packet;

}

BOOL Player::Unpack(std::string& packet)
{
	//解包规则：
    //x|y|n子弹数目(NormalPlayerBullet)|x1|y1|...|xn|yn
	CPoint ptPos;
	//x
	auto it = find(packet.begin(),packet.end(),'|');
	ptPos.x = atoi(std::string(packet.begin(),it).c_str());
	
	
	packet = std::string(it+1,packet.end());
	//y
	it = find(packet.begin(), packet.end(), '|');
	ptPos.y = atoi(std::string(packet.begin(), it).c_str());
	packet = std::string(it + 1, packet.end());

	//设置坐标
	SetPos(ptPos);


	/*
	//clear old bullets
	for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
		delete* it;
		it = m_Bullets.erase(it);
	}

	
	//子弹
	it = find(packet.begin(),packet.end(),'|');
	int n = atoi(std::string(packet.begin(),it).c_str());
	packet = std::string(it + 1, packet.end());

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


		//加子弹
		m_Bullets.push_back(new NormalPlayerBullet(ptPos.x, ptPos.y));

	}
	*/

	return TRUE;
}

std::string Player::mGetClassName()
{
	return m_sClassName;
}
