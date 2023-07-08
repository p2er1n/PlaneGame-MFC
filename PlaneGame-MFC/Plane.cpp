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
	//��ʼ����ʱ����ͼƬ�Ŀ�ߣ��ɰ�ɫ�ʣ�ͼ����ԴID
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

	//���ӵ�
	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ++it) {
		(*it)->Draw(pDC,(*it)->GetImageList());
	}

	return TRUE;
}

BOOL Plane::Load(CImageList& imageList)
{
	//cx cy��ʾÿһ��ͼ��Ĵ�С
	CBitmap bmp;
	bmp.LoadBitmapW(nID);

	//nGrow����Ϊ0��ʾ�����б�����
	if (!imageList.Create(m_iWidth, m_iHeight, ILC_COLOR24 | ILC_MASK, nInitial, 0))
		return FALSE;

	//crMaskָ���ɰ���ɫ
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
	//��������ӵ�
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

	//װ��
	if (m_nCurShootCnt >= m_nShootDelay) {
		LoadBullet();
		m_nCurShootCnt = 0;
	}
	else
		m_nCurShootCnt++;

	//�ƶ��ӵ�
	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ++it) {
		(*it)->Update();
	}
}