#pragma once


// CServicePropertyPage 对话框

class CServicePropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CServicePropertyPage)

public:
	CServicePropertyPage();
	virtual ~CServicePropertyPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_LARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
