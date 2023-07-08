#pragma once
#include "Player.h"

#include <mutex>

class Friend :
    public Player
{
public:
    Friend ();
    Friend(Friend& f);
    virtual CImageList& GetImageList();

    void Update();

protected:
    static CImageList m_ImageList;
    static BOOL m_fLoad;


};

