#include "Plane.h"
#include "MainWnd.h"

CImageList Plane::m_ImageList;
Plane::Plane(int x, int y, int w, int h, COLORREF crM, 
	int nI, UINT ID,UINT speed, UINT shootSpeed,
	UINT health) :
	GameObject(x,y,w,h),
	m_iWidth(w),m_iHeight(h),crMask(crM),
	nInitial(nI),nID(ID),m_nSpeed(speed),
	m_nShootSpeed(shootSpeed),m_nShootDelay(10.0 / m_nShootSpeed * 100),
	m_nHealth(health)
{
	//初始化的时候传入图片的宽高，蒙版色彩，图像资源ID
}

Plane::~Plane()
{
	for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
		delete* it;
		it = m_Bullets.erase(it);
	}
}

BOOL Plane::Draw(CDC* pDC,CImageList& imageList)
{
	if (!imageList.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT))
		AfxMessageBox(L"Draw Plane on DC failed!");

	//画子弹
	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ++it) {
		(*it)->Draw(pDC,(*it)->GetImageList());
	}

	return TRUE;
}

BOOL Plane::Load(CImageList& imageList)
{
	//cx cy表示每一个图像的大小
	CBitmap bmp;
	bmp.LoadBitmapW(nID);

	//nGrow设置为0表示不能列表增长
	if (!imageList.Create(m_iWidth, m_iHeight, ILC_COLOR24 | ILC_MASK, nInitial, 0))
		return FALSE;

	//crMask指定蒙版颜色
	imageList.Add(&bmp,crMask);
	return TRUE;
}

CImageList& Plane::GetImageList()
{
	return m_ImageList;
}

BOOL Plane::Move(int cx, int cy)
{
	CPoint pt(m_ptPos.x + cx, m_ptPos.y + cy);
	return SetPos(pt);
}

std::list<Bullet*>& Plane::GetBullets()
{
	return m_Bullets;
}

UINT Plane::DescreaseHealth(UINT cHealth)
{
	return m_nHealth -= cHealth;
}

int Plane::GetHealth()
{
	return m_nHealth;
}

void Plane::Update()
{
	//清理出界子弹
	for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
		CPoint bPos = (*it)->GetPoint();
		if (bPos.x > GAME_WIDTH || bPos.x < 0 ||
			bPos.y > GAME_HEIGHT || bPos.y < 0) {
			delete* it;
			it = m_Bullets.erase(it);
		}
		else
			it++;
	}

	//装弹
	if (m_nCurShootCnt >= m_nShootDelay) {
		LoadBullet();
		m_nCurShootCnt = 0;
	}
	else
		m_nCurShootCnt++;

	//移动子弹
	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ++it) {
		(*it)->Update();
	}
}