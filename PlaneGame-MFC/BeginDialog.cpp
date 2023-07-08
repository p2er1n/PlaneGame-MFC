#include "BeginDialog.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(BeginDialog,CDialog)
ON_BN_CLICKED(IDC_BUTTON_SINGLE_MODE,OnButtonSingleModeClicked)//������Ϸ
ON_BN_CLICKED(IDC_BUTTON_CREATE_SERVER,OnButtonCreateServerClicked)//����������
ON_BN_CLICKED(IDC_BUTTON_JOIN_GAME, OnButtonJoinGameClicked)//������Ϸ
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
