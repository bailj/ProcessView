// ProcessView.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CProcessViewApp:
// �йش����ʵ�֣������ ProcessView.cpp
//

class CProcessViewApp : public CWinApp
{
public:
	CProcessViewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProcessViewApp theApp;
