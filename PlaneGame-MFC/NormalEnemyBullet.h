#pragma once
#include "Bullet.h"
#include "resource.h"

class NormalEnemyBullet :
    public Bullet
{
public:
    NormalEnemyBullet(int x, int y);
    virtual CImageList& GetImageList();

    virtual std::string mGetClassName();
    virtual std::string Pack();

public:
    static CImageList m_ImageList;
    static BOOL m_fLoad;

    std::string m_sClassName = "NormalEnemyBullet";
};

