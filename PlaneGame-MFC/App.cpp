#include "App.h"
#include "MainWnd.h"
#include "BeginDialog.h"
#include "resource.h"
#include "ServerCreateDialog.h"
#include "ClientConnDialog.h"

BOOL App::InitInstance()
{
	if (!CWinApp::InitInstance())
		return FALSE;

//	AfxMessageBox(L"InitInstance was called!");
	MainWnd* pMainWindow = new MainWnd;
	m_pMainWnd = pMainWindow;

	//��ʼ����Ի���
	BeginDialog bgDlg(IDD_BEGINDIALOG, m_pMainWnd);

	//�����������Ի���
	ServerCreateDialog scDlg(&bgDlg);


	//�ͻ������öԻ���
	ClientConnDialog ccDlg(&bgDlg);

	int iScReturnID;

	switch (bgDlg.DoModal()) {
	case IDCANCEL:
		return FALSE;//return false to make app exit
	case IDSINGLEMODE:
		((MainWnd*)m_pMainWnd)->SetGameMode(mSingle);
		break;
	case IDCREATESERVER:
		((MainWnd*)m_pMainWnd)->SetGameMode(mServer);
		while ((iScReturnID = scDlg.DoModal()) == IDPORTEMPTY) {
			AfxMessageBox(L"�˿ںŲ���Ϊ�գ�");
		}
		//�û�ȡ�������ö˿�
		if (iScReturnID == IDCANCEL)
			return FALSE;

		((MainWnd*)m_pMainWnd)->SetServerPort(scDlg.GetServerPort());
		break;
	case IDJOINGAME:
		((MainWnd*)m_pMainWnd)->SetGameMode(mJoin);
		ccDlg.DoModal();
		((MainWnd*)m_pMainWnd)->SetClientConnIpAddr(ccDlg.GetIpAddr());
		((MainWnd*)m_pMainWnd)->SetClientConnPort(ccDlg.GetPort());
		break;
	}

	((MainWnd*)m_pMainWnd)->Init();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

BOOL App::ExitInstance()
{
	if (!CWinApp::ExitInstance())
		return FALSE;

	return TRUE;
}

App app;