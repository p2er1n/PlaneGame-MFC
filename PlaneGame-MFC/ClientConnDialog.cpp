// ClientConnDialog.cpp: 实现文件
//

#include "afxdialogex.h"
#include "ClientConnDialog.h"

#include "resource.h"

#include <string>

// ClientConnDialog 对话框

IMPLEMENT_DYNAMIC(ClientConnDialog, CDialog)

ClientConnDialog::ClientConnDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CLIENTCONNDIALOG, pParent)
{

}

ClientConnDialog::~ClientConnDialog()
{
}

void ClientConnDialog::OnButtonOkClicked()
{
	CString textContent;
	m_ipEdit.GetWindowTextW(textContent);
	std::string text((char*)CW2A(textContent));
	m_sIp = text;

	m_portEdit.GetWindowTextW(textContent);
	std::string text2((char*)CW2A(textContent));
	m_iPort = atoi(text2.c_str());

	OnOK();
}

std::string ClientConnDialog::GetIpAddr()
{
	return m_sIp;
}

int ClientConnDialog::GetPort()
{
	return m_iPort;
}

void ClientConnDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_CLIENTCONNIP,m_ipEdit);
	DDX_Control(pDX,IDC_CLIENTCONNPORT,m_portEdit);
}


BEGIN_MESSAGE_MAP(ClientConnDialog, CDialog)
	ON_BN_CLICKED(IDOK,OnButtonOkClicked)
END_MESSAGE_MAP()


// ClientConnDialog 消息处理程序
