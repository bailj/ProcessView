// ProcessViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessView.h"
// #include "ProcessViewDlg.h"
#include "ProcessViewPage.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlgX �Ի���
static UINT indicators[]={IDS_PANEL1,IDS_PANEL2};
class CAboutDlgX : public CDialog
{
public:
	CAboutDlgX();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CProcessViewDlg �Ի���

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


// CProcessViewDlg ��Ϣ�������

BOOL CProcessViewPage::OnInitDialog()
{
	CDialog::OnInitDialog();

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
		!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_CListCtrl.GetClientRect(&Rect);

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
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProcessViewPage::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CProcessViewPage::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProcessViewPage::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CProcessViewPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//*
	if(!::IsWindow(m_CListCtrl.GetSafeHwnd()))//��Ϊϵͳ��һ�η���WM_SIZE��ʱ��list��û�д���������Ҫ��һ���ж�
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
	// TODO: �ڴ���������������
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
