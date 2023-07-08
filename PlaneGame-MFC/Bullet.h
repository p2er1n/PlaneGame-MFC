#pragma once
#include "GameObject.h"

/*
* ������
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

    //�����ӵ�ʵ��
    virtual CImageList& GetImageList() = 0;

    virtual BOOL Move(int cx, int cy);
    virtual BOOL Update(); //����λ��

    virtual UINT GetAttack();

public:
    UINT nID;//��ԴID
    int m_iWidth;
    int m_iHeight;
    int nInitial; COLORREF crMask;//��ʼ��CImageList��
    UINT m_nSpeed;

    UINT m_nAttack;//�ӵ��˺�
    int m_iDir[2];//hor ver
};

