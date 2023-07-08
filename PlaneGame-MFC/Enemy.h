#pragma once
#include "Plane.h"
#include "NormalEnemyBullet.h"

/*
* ≥ÈœÛ¿‡
*/

class Enemy : 
    public Plane
{
public:
    Enemy(int x, int y, int speed,UINT nID, int w, int h, UINT shootSpeed, UINT health);
    virtual void Update();
protected:
    int m_iDir[2];//hor ver
};



