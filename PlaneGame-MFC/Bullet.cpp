#include "Bullet.h"

Bullet::Bullet(int x, int y, int w, 
	int h, COLORREF crM, int nI, UINT ID, 
	int speed, UINT attack, int iDirx, int iDiry) :
	GameObject(x, y, w, h), 
	m_iWidth(w), m_iHeight(h), crMask(crM), nInitial(nI), nID(ID), m_nSpeed(speed),
	m_nAttack(attack)
{
	m_iDir[0] = iDirx;
	m_iDir[1] = iDiry;//Ĭ�ϳ�ʼ������
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
	//cx cy��ʾÿһ��ͼ��Ĵ�С
	CBitmap bmp;
	bmp.LoadBitmapW(nID);

	//nGrow����Ϊ0��ʾ�����б�����
	if (!imageList.Create(m_iWidth, m_iHeight, ILC_COLOR24 | ILC_MASK, nInitial, 0))
		return FALSE;

	//crMaskָ���ɰ���ɫ
	imageList.Add(&bmp, crMask);
	return TRUE;
}

BOOL Bullet::isCollision(GameObject* gm)
{
	CRect tmp;
	if (tmp.IntersectRect(gm->GetRect(), m_Rect))//����н�����tmpΪ��
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

