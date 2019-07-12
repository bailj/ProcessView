// ProcessView.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ProcessView.h"
#include "ProcessViewDlg.h"
//#include "ProcessPropertySheet.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessViewApp

BEGIN_MESSAGE_MAP(CProcessViewApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CProcessViewApp ����

CProcessViewApp::CProcessViewApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CProcessViewApp ����

CProcessViewApp theApp;


// CProcessViewApp ��ʼ��

BOOL CProcessViewApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//SetConsole();
	ModifyPrivilege(SE_SECURITY_NAME, TRUE);
	ModifyPrivilege(SE_CREATE_TOKEN_NAME,TRUE);
	ModifyPrivilege(SE_ASSIGNPRIMARYTOKEN_NAME,TRUE);
	ModifyPrivilege(SE_TAKE_OWNERSHIP_NAME,TRUE);
	ModifyPrivilege(SE_LOAD_DRIVER_NAME,TRUE);
	ModifyPrivilege(SE_SYSTEM_PROFILE_NAME,TRUE);
	ModifyPrivilege(SE_INC_BASE_PRIORITY_NAME,TRUE);
	ModifyPrivilege(SE_SYSTEM_ENVIRONMENT_NAME,TRUE);
	ModifyPrivilege(SE_DEBUG_NAME,TRUE);
	//add for 2008 junction
	ModifyPrivilege(SE_BACKUP_NAME,TRUE);//SE_RESTORE_NAME

// 	CProcessPropertySheet pps(_T("Test"));
// 	pps.DoModal();

	CProcessViewDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
