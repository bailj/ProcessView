#pragma once
#include <Windows.h>
bool SetConsole();
void CloseConsole();
HRESULT ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable);

typedef struct _tagProcessNode{
	TCHAR exeFile[1024];
	TCHAR imageName[1024];
	WCHAR commandLine[2560];
	DWORD th32ProcessID;
	DWORD th32ParentProcessID;
	DWORD cntThreads;
	DWORD dwPriorityClass;
	LONG pcPriClassBase;
	DWORD th32ModuleID;
	struct _tagProcessNode* pNext;
}PROCESSNODE,*PPROCESSNODE;

BOOL GetProcessList(PPROCESSNODE* pProcessNode);
void CleanProcessList(PPROCESSNODE* pProcessNode);
void printError( TCHAR* msg );

extern bool g_bConsole;
