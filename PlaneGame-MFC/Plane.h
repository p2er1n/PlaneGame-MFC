#pragma once
#include "GameObject.h"
#include "resource.h"
#include "Bullet.h"

#include <list>
#include <string>
#include <mutex>

/*
* 抽象类
*/

class Plane :
    public GameObject
{
public:
    Plane(int x, int y, int w, int h, COLORREF crM,
        int nI, UINT ID, UINT speed, UINT shootSpeed,
        UINT health);

    virtual ~Plane();

    virtual BOOL Draw(CDC* pDC, CImageList& imageList);
    virtual BOOL Load(CImageList& imageList);
    virtual CImageList& GetImageList();

    virtual void Update();

    virtual void LoadBullet() = 0;

    virtual BOOL Move(int cx, int cy);

    virtual std::list<Bullet*>& GetBullets();

    virtual UINT DescreaseHealth(UINT cHealth);

    virtual int GetHealth();

protected:
    static CImageList m_ImageList;
    int m_iWidth;
    int m_iHeight;
    UINT nID;
    COLORREF crMask;
    int nInitial;

    UINT m_nSpeed;
    UINT m_nShootSpeed;//子弹发射速度

    UINT m_nShootDelay;
    UINT m_nCurShootCnt = 0;

    // 生命值不能用UINT，否则如果攻击过快就会造成数值溢出，生命变得极大
    int m_nHealth;//生命值

    std::list<Bullet*> m_Bullets;
};
