#pragma once
#include "ProcessViewPage.h"
#include "ServicePropertyPage.h"

// CProcessPropertySheet

class CProcessPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CProcessPropertySheet)

public:
	CProcessPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CProcessPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CProcessPropertySheet();
	BOOL OnInitDialog();
protected:
	HICON m_hIcon;
	CStatusBar m_wndStatusBar;
	CStatusBarCtrl* m_StatusBar;

protected:
	void Init();
protected:
	DECLARE_MESSAGE_MAP()

private:
	CProcessViewPage m_CProcessViewPage;
	CServicePropertyPage m_CServicePropertyPage;
};


