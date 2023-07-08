#include "Bullet.h"

Bullet::Bullet(int x, int y, int w, 
	int h, COLORREF crM, int nI, UINT ID, 
	int speed, UINT attack, int iDirx, int iDiry) :
	GameObject(x, y, w, h), 
	m_iWidth(w), m_iHeight(h), crMask(crM), nInitial(nI), nID(ID), m_nSpeed(speed),
	m_nAttack(attack)
{
	m_iDir[0] = iDirx;
	m_iDir[1] = iDiry;//默认初始向下走
}

BOOL Bullet::Draw(CDC* pDC, CImageList& imageList)
{
	if (!imageList.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT)) {
		AfxMessageBox(L"Draw Bullet on DC failed!");
		AfxAbort();
	}
	return TRUE;

}

BOOL Bullet::Load(CImageList& imageList)
{
	//cx cy表示每一个图像的大小
	CBitmap bmp;
	bmp.LoadBitmapW(nID);

	//nGrow设置为0表示不能列表增长
	if (!imageList.Create(m_iWidth, m_iHeight, ILC_COLOR24 | ILC_MASK, nInitial, 0))
		return FALSE;

	//crMask指定蒙版颜色
	imageList.Add(&bmp, crMask);
	return TRUE;
}

BOOL Bullet::isCollision(GameObject* gm)
{
	CRect tmp;
	if (tmp.IntersectRect(gm->GetRect(), m_Rect))//如果有交集，tmp为真
		return TRUE;
	return FALSE;
}

BOOL Bullet::Move(int cx, int cy)
{
	CPoint pt(m_ptPos.x + cx, m_ptPos.y + cy);
	return SetPos(pt);
}

BOOL Bullet::Update()
{
	return Move(m_iDir[0] * m_nSpeed, m_iDir[1] * m_nSpeed);
}

UINT Bullet::GetAttack()
{
	return m_nAttack;
}

