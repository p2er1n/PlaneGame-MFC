#pragma once
#include "afxdialogex.h"

#include <string>

// ClientConnDialog 对话框

class ClientConnDialog : public CDialog
{
	DECLARE_DYNAMIC(ClientConnDialog)

public:
	ClientConnDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ClientConnDialog();

	afx_msg void OnButtonOkClicked();

	std::string GetIpAddr();
	int GetPort();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTCONNDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CEdit m_ipEdit;
	CEdit m_portEdit;

	std::string m_sIp;
	int m_iPort;

	DECLARE_MESSAGE_MAP()
};
