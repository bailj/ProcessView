#pragma once


// CServicePropertyPage �Ի���

class CServicePropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CServicePropertyPage)

public:
	CServicePropertyPage();
	virtual ~CServicePropertyPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_LARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
