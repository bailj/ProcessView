// ProcessViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessView.h"
// #include "ProcessViewDlg.h"
#include "ProcessViewPage.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlgX 对话框
static UINT indicators[]={IDS_PANEL1,IDS_PANEL2};
class CAboutDlgX : public CDialog
{
public:
	CAboutDlgX();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlgX::CAboutDlgX() : CDialog(CAboutDlgX::IDD)
{
}

void CAboutDlgX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlgX, CDialog)
END_MESSAGE_MAP()


// CProcessViewDlg 对话框

IMPLEMENT_DYNAMIC(CProcessViewPage, CPropertyPage)

CProcessViewPage::CProcessViewPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CProcessViewPage::IDD)
{
	m_StatusBar=NULL;
	m_bRunning=false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CProcessViewPage::~CProcessViewPage(){
	if(m_StatusBar)
		delete m_StatusBar;
}
void CProcessViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CListCtrl);
}

BEGIN_MESSAGE_MAP(CProcessViewPage, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_STARTREFRESH, OnFileStartrefresh)
	ON_UPDATE_COMMAND_UI(ID_FILE_STARTREFRESH, OnUpdateFileStartrefresh)
END_MESSAGE_MAP()


// CProcessViewDlg 消息处理程序

BOOL CProcessViewPage::OnInitDialog()
{
	CDialog::OnInitDialog();

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
		!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_CListCtrl.GetClientRect(&Rect);

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

	m_CListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_CListCtrl.InsertColumn(0, TEXT("PID"), LVCFMT_LEFT, Rect.Width() /8);
	m_CListCtrl.InsertColumn(1, TEXT("PPID"), LVCFMT_LEFT, Rect.Width() /8);
	m_CListCtrl.InsertColumn(2, TEXT("ExeFile"), LVCFMT_LEFT, Rect.Width() / 5);
	m_CListCtrl.InsertColumn(3, TEXT("cntThreads"), LVCFMT_LEFT, Rect.Width() / 5);
	m_CListCtrl.InsertColumn(4, TEXT("Cmd Line"), LVCFMT_LEFT, Rect.Width() / 4);
	m_CListCtrl.InsertColumn(5, TEXT("Image"), LVCFMT_LEFT, Rect.Width() / 4);
	

	CenterWindow();
	GetWindowRect(&Rect);
	Rect.top+=1;
	Rect.right+=1;
	MoveWindow(Rect);
	ShowWindow(SW_SHOW);
	InitProcessList();
	SetTimer(1,2000,NULL);
	SetTimer(2,1000,NULL);

	m_bRunning=true;
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CProcessViewPage::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlgX dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessViewPage::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CProcessViewPage::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProcessViewPage::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
void CProcessViewPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//*
	if(!::IsWindow(m_CListCtrl.GetSafeHwnd()))//因为系统第一次发送WM_SIZE的时候，list还没有创建，所以要做一下判断
		return ;
	CRect rect,rect2;
	GetClientRect(rect);
	m_CListCtrl.MoveWindow(rect.left, rect.top, rect.right, rect.bottom-25);
	//m_wndStatusBar.MoveWindow(2,rect.bottom-25,rect.right,25);
	m_StatusBar->MoveWindow(2,rect.bottom-25,rect.right,25);
	//*/
}
void CProcessViewPage::OnFileExit()
{
	// TODO: 在此添加命令处理程序代码
	this->OnCancel();
}
void CProcessViewPage::InitProcessList(){
	PPROCESSNODE p;
	while (m_CListCtrl.GetItemCount()){
		m_CListCtrl.DeleteItem(0);
	}

	if(GetProcessList(&p)){
		PPROCESSNODE t=p;
		CString cs;
		int j=0;
		while(t){
			cs.Format(_T("%d"),t->th32ProcessID);
			m_CListCtrl.InsertItem(j,cs);
			cs.Format(_T("%d"),t->th32ParentProcessID);
			m_CListCtrl.SetItemText(j,1,cs);
			cs.Format(_T("%s"),t->exeFile);
			m_CListCtrl.SetItemText(j,2,cs);
			cs.Format(_T("%d"),t->cntThreads);
			m_CListCtrl.SetItemText(j,3,cs);
			cs.Format(_T("%s"),t->commandLine);
			m_CListCtrl.SetItemText(j,4,cs);
			cs.Format(_T("%s"),t->imageName);
			m_CListCtrl.SetItemText(j,5,cs);
			j++;
			t=t->pNext;
		}
		CleanProcessList(&p);
	}
}
void CProcessViewPage::OnTimer(UINT nIDEvent){
	if(nIDEvent==1)
		InitProcessList();
	else if(nIDEvent==2){
		CTime now=CTime::GetCurrentTime();
		CString c=now.Format(_T("%H:%m:%S"));
		m_StatusBar->SetText(c,0,0);
		m_StatusBar->SetText(c,2,0);
		m_StatusBar->UpdateData(TRUE);
	}

}
void CProcessViewPage::OnFileStartrefresh()
{
	if(m_bRunning){
		KillTimer(1);
		m_bRunning=false;
	}else{
		SetTimer(1,2000,NULL);
		m_bRunning=true;
	}
}

void CProcessViewPage::OnUpdateFileStartrefresh(CCmdUI *pCmdUI)
{
	if(m_bRunning)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(2);
}
