#pragma once
#include <afxwin.h>

#define IDPORTEMPTY 2000

class ServerCreateDialog : public CDialog
{
public:
	ServerCreateDialog(CWnd* pParent = nullptr);

	int GetServerPort();

	void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnButtonOkClicked();

private:
	CEdit m_editPort;
	int m_iPort;
	DECLARE_MESSAGE_MAP()
};
