
#ifndef __REDIR_HTTP_H__
#define __REDIR_HTTP_H__

#include "cstring.h"
#include "redirect.h"
#include "utility.h"
#include "ssllib.h"

class CRedirectHTTP;
class CRedirectHTTP_Thread;

class CRedirectHTTP_Thread : public CRedirectBase
{
public:
	CRedirectHTTP_Thread();
	virtual void StartRedirect();
	virtual ~CRedirectHTTP_Thread() throw() { }

	CRedirectHTTP				*m_pRedirHTTP;

	CSSLSocket					 m_csslSocket;		// The SSL socket
	int							 m_iConnections;
	int							 m_sClientSocket;
	int							 m_sServerSocket;
};

class CRedirectHTTP : public CRedirectBase
{
public:
	CRedirectHTTP();
	virtual void StartRedirect();
	virtual ~CRedirectHTTP() throw() { }

	void HTTPProxy();

	bool						 m_bUseSSL;
	int							 m_sListenSocket;
};

#endif // __REDIR_HTTP_H__
