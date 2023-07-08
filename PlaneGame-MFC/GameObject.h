#pragma once
#include <afxwin.h>
#include <string>

/*
* �����࣬����֮ʼ
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

    //ÿ����������붨���Լ��ľ�̬ImageList�����ͼƬ����
    virtual BOOL Load(CImageList& imageList) = 0;
    
    virtual std::string Pack() = 0;

    virtual std::string mGetClassName() = 0;

protected:
    CPoint m_ptPos;
    CRect m_Rect;
};

