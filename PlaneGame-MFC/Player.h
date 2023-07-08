#pragma once
#include "Plane.h"

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 60

class Player :
    public Plane
{
public:
    Player(int x, int y, int speed);
    virtual CImageList& GetImageList();
    virtual BOOL Move(int cx, int cy);

    virtual void Update();

    virtual void LoadBullet();

    virtual std::string Pack();
    virtual BOOL Unpack(std::string&packet);

    virtual std::string mGetClassName();

protected:
    static CImageList m_ImageList;
    static BOOL m_fLoad;
    std::string m_sClassName = "Player";
};



