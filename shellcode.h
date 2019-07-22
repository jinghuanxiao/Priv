
#ifndef __SHELLCODE_H__
#define __SHELLCODE_H__

typedef bool (*SCCallbackFunc)(char *szShellBuf, int iShellBufSize);

bool contains(char *szBuf, int iSize, char cChar);
int setup_shellcode(char *szOrigShell, int iOrigShellSize, char *szShellBuf, int iShellBufSize, int iPort, int iHost, int iPortOffset, int iHostOffset, SCCallbackFunc pfnSC);
int encrypt_shellcode(char *szOrigShell, int iOrigShellSize, char *szShellBuf, int iShellBufSize, SCCallbackFunc pfnSC);

#endif // __SHELLCODE_H__
