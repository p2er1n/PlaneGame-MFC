#include "NormalPlayerBullet.h"

CImageList NormalPlayerBullet::m_ImageList;
BOOL NormalPlayerBullet::m_fLoad = FALSE;

static const UINT s_attack = 5;
static const int s_speed = 10;

NormalPlayerBullet::NormalPlayerBullet(int x, int y) :
	Bullet(x, y, 10, 20, RGB(0, 0, 0), 2, IDB_NORMALPLAYERBULLET, s_speed, s_attack, 0, -1)
{
	if (!m_fLoad) {
		Load(m_ImageList);
		m_fLoad = TRUE;
	}
}

CImageList& NormalPlayerBullet::GetImageList()
{
	return m_ImageList;
}

std::string NormalPlayerBullet::mGetClassName()
{
	return m_sClassName;
}

std::string NormalPlayerBullet::Pack()
{
	return "Undefined function";
}
