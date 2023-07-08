#include "Friend.h"

#include "NormalPlayerBullet.h"

CImageList Friend::m_ImageList;
BOOL Friend::m_fLoad = FALSE;
Friend::Friend() :
	Player(0, 0, 0)
{
	if (!m_fLoad) {
		Load(m_ImageList);
		m_fLoad = TRUE;
	}
}

Friend::Friend(Friend& f) :
	Friend()
{

	SetPos(f.GetPoint());

	for (auto it = f.m_Bullets.begin();it != f.m_Bullets.end();++it) {
		CPoint ptPos = (*it)->GetPoint();
		m_Bullets.push_back(new NormalPlayerBullet(ptPos.x, ptPos.y));
	}
}

CImageList& Friend::GetImageList()
{
	return m_ImageList;
}

void Friend::Update()
{
	Plane::Update();
}
