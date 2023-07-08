#include "ServerCreateDialog.h"
#include "resource.h"

#include <string>

BEGIN_MESSAGE_MAP(ServerCreateDialog, CDialog)
	ON_BN_CLICKED(IDOK,OnButtonOkClicked)
END_MESSAGE_MAP()

ServerCreateDialog::ServerCreateDialog(CWnd* pParent)
	: CDialog(IDD_SERVERCREATEDILOG, pParent)
{

}

int ServerCreateDialog::GetServerPort()
{
	return m_iPort;
}

void ServerCreateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_PORT,m_editPort);
}

void ServerCreateDialog::OnButtonOkClicked()
{
	CString textContent;
	m_editPort.GetWindowTextW(textContent);

	std::string text((char *)CW2A(textContent));

	if (text.empty()) {
		EndDialog(IDPORTEMPTY);
	}
	else {
		m_iPort = atoi(text.c_str());
		OnOK();
	}
}
