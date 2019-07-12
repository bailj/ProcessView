// ServicePropertyPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessView.h"
#include "ServicePropertyPage.h"


// CServicePropertyPage 对话框

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


// CServicePropertyPage 消息处理程序
