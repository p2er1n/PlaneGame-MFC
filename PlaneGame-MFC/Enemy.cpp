#include "Enemy.h"

Enemy::Enemy(int x, int y, int speed, UINT nID, int w, int h, UINT shootSpeed, UINT health) :
	Plane(x, y, w, h, RGB(0, 0, 0), 2, nID, speed, shootSpeed, health)
{
	m_iDir[0] = 0;//Ĭ��ˮƽ���ƶ�
	m_iDir[1] = 1;//Ĭ����ֱ������
}

void Enemy::Update()
{
	Plane::Update();

	Move(m_iDir[0] * m_nSpeed, m_iDir[1] * m_nSpeed);
}





