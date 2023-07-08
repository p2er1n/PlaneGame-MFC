#pragma once
#include <afxwin.h>

#define IDSINGLEMODE 1000
#define IDCREATESERVER 1001
#define IDJOINGAME 1002

class BeginDialog :
    public CDialog
{
public:
    BeginDialog(UINT,CWnd*);

    afx_msg void OnButtonSingleModeClicked();
    afx_msg void OnButtonCreateServerClicked();
    afx_msg void OnButtonJoinGameClicked();

    //member variables
public:
    UINT m_nIDTemplate;

    DECLARE_MESSAGE_MAP()
};

