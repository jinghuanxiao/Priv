
#include "main.h"
#include "sdcompat.h"
#include "mainctrl.h"

void CsdbotCompat::Init() {
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdTest,	"sd.test",	"test",	this);
}

int CsdbotCompat::Compat(char *a[256], bool notice, bool silent) {
	char sendbuf[512];

	return 0;
}

bool CsdbotCompat::HandleCommand(CMessage *pMsg) {
#ifdef DBGCONSOLE
        g_cMainCtrl.m_cConsDbg.Log(10, "File =%s:Line=%d:Function=%s \n",__FILE__, __LINE__, __FUNCTION__);
        g_cMainCtrl.m_cConsDbg.Log(10, "*%s* %s  \n", pMsg->sSrc.CStr(), pMsg->sChatString.CStr());
#endif
	return false; }
