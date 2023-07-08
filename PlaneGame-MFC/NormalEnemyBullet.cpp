#include "NormalEnemyBullet.h"

CImageList NormalEnemyBullet::m_ImageList;
BOOL NormalEnemyBullet::m_fLoad = FALSE;

static const UINT s_attack = 2;
static const int s_speed = 10;

NormalEnemyBullet::NormalEnemyBullet(int x, int y) :
	Bullet(x, y, 8, 8, RGB(0, 0, 0), 2, IDB_NORMALENEMYBULLET, s_speed,s_attack, 0, 1)
{
	if (!m_fLoad) {
		Load(m_ImageList);
		m_fLoad = TRUE;
	}
}

CImageList& NormalEnemyBullet::GetImageList()
{
	return m_ImageList;
}

std::string NormalEnemyBullet::mGetClassName()
{
	return m_sClassName;
}

std::string NormalEnemyBullet::Pack()
{
	return "Undefined function";
}

