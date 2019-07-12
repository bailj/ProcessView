// ProcessPropertySheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessView.h"
#include "ProcessPropertySheet.h"


// CProcessPropertySheet
static UINT indicatorss[]={IDS_PANEL1,IDS_PANEL2};

IMPLEMENT_DYNAMIC(CProcessPropertySheet, CPropertySheet)
CProcessPropertySheet::CProcessPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CProcessPropertySheet::CProcessPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

CProcessPropertySheet::~CProcessPropertySheet()
{
}
void CProcessPropertySheet::Init(){
	AddPage(&m_CProcessViewPage);
	AddPage(&m_CServicePropertyPage);
}
BOOL CProcessPropertySheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CRect Rect,Rect2;
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicatorss,sizeof(indicatorss)/sizeof(UINT))
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
// 	m_CListCtrl.GetClientRect(&Rect);

	//״̬��1;
	m_wndStatusBar.GetWindowRect(&Rect2);
	m_wndStatusBar.MoveWindow(0,Rect.bottom-20,Rect.right,20);
	m_wndStatusBar.SetPaneText(0,_T("X:"));
	m_wndStatusBar.SetPaneText(1,_T("Y:"));


	//״̬��2
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[4]= {100, 200, 320, -1}; //�ָ�����
	m_StatusBar->SetParts(4, strPartDim);
	m_StatusBar->SetText(_T("�������"),0,0); //��һ����������"�������",��������
	m_StatusBar->SetText(_T("��ӭ��Ĺ���"), 1, 0);
	m_StatusBar->SetText(_T("�������"), 2, 0);
	m_StatusBar->SetText(_T("������, QQ: 27269269"), 3, 0);
	//��������״̬���м���ͼ��
	m_StatusBar->SetIcon(3,
		SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),
		FALSE));//�ڵ��ĸ������м���IDΪIDI_ICON3��ͼ��//----------�������---------------------
	//�����ı�ĳ��������Ŀ����,������������ƴ��룺
	//m_StatusBar->SetText("��������", 2, 0);
	//���磬������ı��һ����������������Ϊ������æ������������
	//m_StatusBar->SetText(_T("����æ"), 0, 0); 

	//�˵�Ҳ�ɲ�������ķ�ʽ
	// 	m_CMenuMain.LoadMenu(IDR_MAINFRAME);
	// 	SetMenu(&m_CMenuMain);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}
BEGIN_MESSAGE_MAP(CProcessPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CProcessPropertySheet ��Ϣ�������
