// ProcessViewDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ProcessPropertySheet.h"

// CProcessViewDlg 对话框
class CProcessViewDlg : public CDialog
{
// 构造
public:
	CProcessViewDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CProcessViewDlg();	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROCESSVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void InitProcessList();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_CListCtrl;
	CStatusBar m_wndStatusBar;

	CProcessPropertySheet m_ppt;

	CStatusBarCtrl* m_StatusBar;
	bool m_bRunning;

	afx_msg void OnFileExit();
	afx_msg void OnFileStartrefresh();
	afx_msg void OnUpdateFileStartrefresh(CCmdUI *pCmdUI);
};
