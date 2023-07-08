#include "BeginDialog.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(BeginDialog,CDialog)
ON_BN_CLICKED(IDC_BUTTON_SINGLE_MODE,OnButtonSingleModeClicked)//单人游戏
ON_BN_CLICKED(IDC_BUTTON_CREATE_SERVER,OnButtonCreateServerClicked)//创建服务器
ON_BN_CLICKED(IDC_BUTTON_JOIN_GAME, OnButtonJoinGameClicked)//加入游戏
	END_MESSAGE_MAP()

BeginDialog::BeginDialog(UINT nIDTemplate,CWnd* pParent):
	CDialog(nIDTemplate,pParent),m_nIDTemplate(nIDTemplate)
{
	
}

void BeginDialog::OnButtonSingleModeClicked()
{
	EndDialog(IDSINGLEMODE);
}

void BeginDialog::OnButtonCreateServerClicked()
{
	EndDialog(IDCREATESERVER);
}

void BeginDialog::OnButtonJoinGameClicked()
{
	EndDialog(IDJOINGAME);
}
