#pragma once
#include <afxwin.h>
#include <string>

/*
* 抽象类，万物之始
*/

class GameObject :
    public CObject
{
public:
    GameObject(int x, int y, int w, int h);
    virtual CRect GetRect() const;
    virtual CPoint GetPoint() const;
    virtual BOOL Draw(CDC* pDC, CImageList& imageList) = 0;
    virtual CImageList& GetImageList() = 0;
    virtual BOOL SetPos(CPoint);

    //每个派生类必须定义自己的静态ImageList来存放图片数据
    virtual BOOL Load(CImageList& imageList) = 0;
    
    virtual std::string Pack() = 0;

    virtual std::string mGetClassName() = 0;

protected:
    CPoint m_ptPos;
    CRect m_Rect;
};

