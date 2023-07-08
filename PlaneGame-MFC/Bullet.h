#pragma once
#include "GameObject.h"

/*
* 抽象类
*/

class Bullet :
    public GameObject
{
public:
    Bullet(int x, int y, int w, int h, 
        COLORREF crM, int nI, UINT ID, int speed, 
        UINT attack, int iDirx, int iDiry);
    virtual BOOL Draw(CDC* pDC, CImageList& imageList);
    virtual BOOL Load(CImageList& imageList);

	virtual BOOL isCollision(GameObject* gm);

    //具体子弹实现
    virtual CImageList& GetImageList() = 0;

    virtual BOOL Move(int cx, int cy);
    virtual BOOL Update(); //更新位置

    virtual UINT GetAttack();

public:
    UINT nID;//资源ID
    int m_iWidth;
    int m_iHeight;
    int nInitial; COLORREF crMask;//初始化CImageList用
    UINT m_nSpeed;

    UINT m_nAttack;//子弹伤害
    int m_iDir[2];//hor ver
};

