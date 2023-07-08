#include "GameObject.h"

GameObject::GameObject(int x, int y, int w, int h) :
	m_ptPos(x, y), m_Rect(x,y,x+w,y+h)
{
}

CRect GameObject::GetRect() const
{
	return m_Rect;
}

CPoint GameObject::GetPoint() const
{
	return m_ptPos;
}

BOOL GameObject::SetPos(CPoint p)
{
	m_ptPos = p;
	m_Rect.MoveToXY(m_ptPos);
	return TRUE;
}

