#pragma once
#include "Enemy.h"

class NormalEnemy :
    public Enemy
{
public:
    NormalEnemy(int x, int y);
    NormalEnemy(const NormalEnemy&);

	virtual CImageList& GetImageList();

    virtual void LoadBullet();

    virtual std::string Pack();
    virtual BOOL Unpack(std::string&);

    virtual std::string mGetClassName();

public:
    static CImageList m_ImageList;
    static BOOL m_fLoad;

    std::string m_sClassName = "NormalEnemy";
};

