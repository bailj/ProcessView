#include "StdAfx.h"
#include "Tools.h"
#include <psapi.h>

#include <Tlhelp32.h>
#include <Winternl.h>
//#include <stierr.h>
//#include <Winsvc.h>
//#include <ntstatus.h>
//#include <wdm.h>

BOOL ListProcessModules( DWORD dwPID );
BOOL ListProcessThreads( DWORD dwOwnerPID );

static bool g_bConsole=false;
bool SetConsole() {
    BOOL bConsole=AllocConsole();
    const TCHAR* ctitle=_T("mars syscopy agent");
    if(bConsole) {
        SetConsoleTitle(ctitle);
        HWND   hwnd   =   NULL;
        while(NULL==hwnd)
            hwnd =::FindWindow(NULL,(LPCTSTR)ctitle)   ;
        //Remove   the   Close   menu   item.   This   will   also   disable   the   [X]   button   in   the   title   bar.
        HMENU   hmenu   =   ::GetSystemMenu (   hwnd,   FALSE   );
        DeleteMenu   (hmenu, SC_CLOSE,   MF_BYCOMMAND   );

        //----------------------------------------------------
        //   设置控制台图标
        //SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
        //SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
        // 		CWnd   *pWnd   =   CWnd::FromHandle(hwnd);
        // 		pWnd-> SetIcon(AfxGetApp()-> LoadStandardIcon(IDI_ASTERISK),FALSE);


        freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
        freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT
        g_bConsole=true;
        return true;
    }
    return false;
}
void CloseConsole() {
    if(g_bConsole) {
        FreeConsole();
        g_bConsole=false;
    }
}

HRESULT ModifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable) {
    HRESULT hr = S_OK;
    TOKEN_PRIVILEGES NewState;
    LUID             luid;
    HANDLE hToken    = NULL;

    // Open the process token for this process.
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ))
        return ERROR_CALL_NOT_IMPLEMENTED;

    // Get the local unique id for the privilege.
    if ( !LookupPrivilegeValue( NULL, szPrivilege, &luid )) {
        CloseHandle( hToken );
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    // Assign values to the TOKEN_PRIVILEGE structure.
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Luid = luid;
    NewState.Privileges[0].Attributes = (fEnable ? SE_PRIVILEGE_ENABLED : 0);

    // Adjust the token privilege.
    if (!AdjustTokenPrivileges(hToken, FALSE, &NewState, sizeof(NewState), NULL, NULL))
        hr = ERROR_CALL_NOT_IMPLEMENTED;

    // Close the handle.
    CloseHandle(hToken);
    return hr;
}

//http://topic.csdn.net/u/20070123/15/12b24a4b-4058-427e-be45-e5b0d44ed18d.html
//http://www.cnblogs.com/BoyXiao/archive/2011/02/27/1966383.html
typedef NTSTATUS (WINAPI *PNtQueryInformationProcess)(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);
typedef struct {
    ULONG          AllocationSize;
    ULONG          ActualSize;
    ULONG          Flags;
    ULONG          Unknown1;
    UNICODE_STRING Unknown2;
    HANDLE         InputHandle;
    HANDLE         OutputHandle;
    HANDLE         ErrorHandle;
    UNICODE_STRING CurrentDirectory;
    HANDLE         CurrentDirectoryHandle;
    UNICODE_STRING SearchPaths;
    UNICODE_STRING ApplicationName;
    UNICODE_STRING CommandLine;
    PVOID          EnvironmentBlock;
    ULONG          Unknown[9];
    UNICODE_STRING Unknown3;
    UNICODE_STRING Unknown4;
    UNICODE_STRING Unknown5;
    UNICODE_STRING Unknown6;
}
PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;

typedef struct {
    ULONG               AllocationSize;
    ULONG               Unknown1;
    HINSTANCE           ProcessHinstance;
    PVOID               ListDlls;
    PPROCESS_PARAMETERS ProcessParameters;
    ULONG               Unknown2;
    HANDLE              Heap;
}
XPEB, *PXPEB;

typedef struct {
    DWORD ExitStatus;
    PPEB  PebBaseAddress;
    DWORD AffinityMask;
    DWORD BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
}
XPROCESS_BASIC_INFORMATION;

BOOL GetCommandLine(HANDLE hProcess,LPWSTR wBuf,DWORD* pdwBufLen,PNtQueryInformationProcess pNtQueryInformationProcess) {
    LONG                      status;
    PROCESS_BASIC_INFORMATION pbi;
    //PEB                       Peb;
    PROCESS_PARAMETERS        ProcParam;
    DWORD                     dwDummy;
    DWORD                     dwSize;
    LPVOID                    lpAddress;
    BOOL                      bRet = FALSE;

    // Retrieve information
    status = pNtQueryInformationProcess(
                 hProcess,
                 ProcessBasicInformation,
                 (PVOID)&pbi,
                 sizeof(PROCESS_BASIC_INFORMATION),
                 NULL
             );
    if (status)
        goto cleanup;

    XPEB xPeb;
    if (!ReadProcessMemory( hProcess,
                            pbi.PebBaseAddress,
                            &xPeb,
                            sizeof(XPEB),
                            &dwDummy
                          )
       )
        goto cleanup;

    if (!ReadProcessMemory(
                hProcess,
                xPeb.ProcessParameters,
                &ProcParam,
                sizeof(PROCESS_PARAMETERS),
                &dwDummy
            )
       )
        goto cleanup;

    lpAddress = ProcParam.CommandLine.Buffer;
    dwSize = ProcParam.CommandLine.Length;

    if (*pdwBufLen<dwSize)
        goto cleanup;

    if (!ReadProcessMemory( hProcess,
                            lpAddress,
                            wBuf,
                            dwSize,
                            pdwBufLen
                          ))
        goto cleanup;


    bRet = TRUE;
    return bRet;
cleanup:
    printError(_T("xxx"));
    return FALSE;
}


BOOL GetProcessList(PPROCESSNODE* pProcessNode) {
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    *pProcessNode=NULL;
    HINSTANCE hnt= LoadLibrary(_T("ntdll.dll"));
    PNtQueryInformationProcess pNtQueryInformationProcess=(PNtQueryInformationProcess)GetProcAddress(hnt,"NtQueryInformationProcess");

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE ) {
        printError( _T("CreateToolhelp32Snapshot (of processes)") );
        return( FALSE );
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &pe32 ) ) {
        printError( _T("Process32First") ); // Show cause of failure
        CloseHandle( hProcessSnap );    // Must clean up the
        //   snapshot object!
        return( FALSE );
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
	PPROCESSNODE pH=NULL,pP=NULL;
    do {
        PPROCESSNODE p=new PROCESSNODE;
		memset(p,0,sizeof(PROCESSNODE));
        _tprintf( _T("\n\n=====================================================") );
        _tprintf( _T("\nPROCESS NAME:  %s"), pe32.szExeFile );
        _tprintf( _T("\n-----------------------------------------------------") );
        _tcscpy(p->exeFile,pe32.szExeFile);

        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(
                       //PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION ,
                       PROCESS_ALL_ACCESS,
                       FALSE, pe32.th32ProcessID );
        if( hProcess == NULL )
            printError( _T("OpenProcess") );
        else {
            TCHAR buf[1024];
            DWORD dwSize=1024;
            //QueryFullProcessImageName(hProcess,0,buf,&dwSize); //for win7
            if(GetProcessImageFileName(hProcess,buf,dwSize)>0) {
                _tprintf(_T("\n  image:%s\n"),buf);
                _tcscpy(p->imageName,buf);
            } else {
                printError(_T("Get image"));
            }
            dwPriorityClass = GetPriorityClass( hProcess );
            if( !dwPriorityClass )
                printError( _T("GetPriorityClass") );
            WCHAR wstr[2560];
            DWORD len=sizeof(wstr);
            memset(wstr,0,len);
            if(GetCommandLine(hProcess,wstr,&len,pNtQueryInformationProcess)) {
                _tprintf(_T("\ncommand:%s"),wstr);
                wcscpy(p->commandLine,wstr);
            }
            CloseHandle( hProcess );
        }

        //*
        printf( "\n  process ID        = 0x%08X", pe32.th32ProcessID );
        printf( "\n  thread count      = %d",   pe32.cntThreads );
        printf( "\n  parent process ID = 0x%08X",pe32.th32ParentProcessID );
        printf( "\n  Priority Base     = %d", pe32.pcPriClassBase );
        if( dwPriorityClass )
            printf( "\n  Priority Class    = %d", dwPriorityClass );
        printf( "\n  th32ModuleID     = %d", pe32.th32ModuleID);

        p->th32ProcessID=pe32.th32ProcessID;
        p->th32ParentProcessID=pe32.th32ParentProcessID;
        p->cntThreads=pe32.cntThreads;
        p->dwPriorityClass=dwPriorityClass;
        p->pcPriClassBase=pe32.pcPriClassBase;

        // List the modules and threads associated with this process
        //ListProcessModules( pe32.th32ProcessID );
        //ListProcessThreads( pe32.th32ProcessID );

		if(pH)
			pP->pNext=p;
		else
			pH=p;
		pP=p;
    } while( Process32Next( hProcessSnap, &pe32 ) );

    CloseHandle( hProcessSnap );
	*pProcessNode=pH;
    return( TRUE );
}

void CleanProcessList(PPROCESSNODE* pProcessNode) {
    PPROCESSNODE p=*pProcessNode;
    while(p) {
        PPROCESSNODE n=p->pNext;
        delete p;
        p=n;
    }
}

BOOL ListProcessModules( DWORD dwPID ) {
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
    if( hModuleSnap == INVALID_HANDLE_VALUE ) {
        printError( _T("CreateToolhelp32Snapshot (of modules)") );
        return( FALSE );
    }

    // Set the size of the structure before using it.
    me32.dwSize = sizeof( MODULEENTRY32 );

    // Retrieve information about the first module,
    // and exit if unsuccessful
    if( !Module32First( hModuleSnap, &me32 ) ) {
        printError( _T("Module32First") ); // Show cause of failure
        CloseHandle( hModuleSnap );    // Must clean up the
        //   snapshot object!
        return( FALSE );
    }

    // Now walk the module list of the process,
    // and display information about each module
    do {
        _tprintf( _T("\n\n     MODULE NAME:     %s"),
                  me32.szModule );
        _tprintf( _T("\n     executable     = %s"),
                  me32.szExePath );
        printf( "\n     process ID     = 0x%08X",
                me32.th32ProcessID );
        printf( "\n     ref count (g)  =     0x%04X",
                me32.GlblcntUsage );
        printf( "\n     ref count (p)  =     0x%04X",
                me32.ProccntUsage );
        printf( "\n     base address   = 0x%08X",
                (DWORD) me32.modBaseAddr );
        printf( "\n     base size      = %d",
                me32.modBaseSize );

    } while( Module32Next( hModuleSnap, &me32 ) );

    CloseHandle( hModuleSnap );
    return( TRUE );
}

BOOL ListProcessThreads( DWORD dwOwnerPID ) {
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads
    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
    if( hThreadSnap == INVALID_HANDLE_VALUE )
        return( FALSE );

    // Fill in the size of the structure before using it.
    te32.dwSize = sizeof(THREADENTRY32 );

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if( !Thread32First( hThreadSnap, &te32 ) ) {
        printError( _T("Thread32First") ); // Show cause of failure
        CloseHandle( hThreadSnap );    // Must clean up the
        //   snapshot object!
        return( FALSE );
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do {
        if( te32.th32OwnerProcessID == dwOwnerPID ) {
            printf( "\n\n     THREAD ID      = 0x%08X",
                    te32.th32ThreadID );
            printf( "\n     base priority  = %d", te32.tpBasePri );
            printf( "\n     delta priority = %d", te32.tpDeltaPri );
        }
    } while( Thread32Next(hThreadSnap, &te32 ) );

    CloseHandle( hThreadSnap );
    return( TRUE );
}

void printError( TCHAR* msg ) {
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError( );
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default lang.
        sysMsg, 256, NULL );

    //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do {
        *p-- = 0;
    } while( ( p >= sysMsg ) &&
             ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    _tprintf( _T("\n  WARNING: %s failed with error %d (%s)"),
              msg, eNum, sysMsg );
}

