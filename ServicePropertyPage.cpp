// ServicePropertyPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessView.h"
#include "ServicePropertyPage.h"


// CServicePropertyPage �Ի���

IMPLEMENT_DYNAMIC(CServicePropertyPage, CPropertyPage)
CServicePropertyPage::CServicePropertyPage()
	: CPropertyPage(CServicePropertyPage::IDD)
{
}

CServicePropertyPage::~CServicePropertyPage()
{
}

void CServicePropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServicePropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CServicePropertyPage ��Ϣ�������
