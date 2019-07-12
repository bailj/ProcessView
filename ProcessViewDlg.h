// ProcessViewDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ProcessPropertySheet.h"

// CProcessViewDlg �Ի���
class CProcessViewDlg : public CDialog
{
// ����
public:
	CProcessViewDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CProcessViewDlg();	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROCESSVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void InitProcessList();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
