// ProcessPropertySheet.cpp : 实现文件
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

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CRect Rect,Rect2;
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicatorss,sizeof(indicatorss)/sizeof(UINT))
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
// 	m_CListCtrl.GetClientRect(&Rect);

	//状态栏1;
	m_wndStatusBar.GetWindowRect(&Rect2);
	m_wndStatusBar.MoveWindow(0,Rect.bottom-20,Rect.right,20);
	m_wndStatusBar.SetPaneText(0,_T("X:"));
	m_wndStatusBar.SetPaneText(1,_T("Y:"));


	//状态栏2
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[4]= {100, 200, 320, -1}; //分割数量
	m_StatusBar->SetParts(4, strPartDim);
	m_StatusBar->SetText(_T("程序就绪"),0,0); //第一个分栏加入"程序就绪",以下类似
	m_StatusBar->SetText(_T("欢迎你的光临"), 1, 0);
	m_StatusBar->SetText(_T("哈哈你好"), 2, 0);
	m_StatusBar->SetText(_T("白立军, QQ: 27269269"), 3, 0);
	//下面是在状态栏中加入图标
	m_StatusBar->SetIcon(3,
		SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),
		FALSE));//在第四个分栏中加入ID为IDI_ICON3的图标//----------代码结束---------------------
	//如果想改变某个分栏项目文字,请调用如下类似代码：
	//m_StatusBar->SetText("文字内容", 2, 0);
	//比如，我们想改变第一个分栏的文字内容为“程序繁忙”，代码如下
	//m_StatusBar->SetText(_T("程序繁忙"), 0, 0); 

	//菜单也可采用下面的方式
	// 	m_CMenuMain.LoadMenu(IDR_MAINFRAME);
	// 	SetMenu(&m_CMenuMain);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}
BEGIN_MESSAGE_MAP(CProcessPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CProcessPropertySheet 消息处理程序
