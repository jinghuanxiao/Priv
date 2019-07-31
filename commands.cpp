
#include "main.h"
#include "commands.h"
#include "mainctrl.h"

CCommands::CCommands()
{
    RegisterCommand(&m_cmdList, "commands.list", "Lists all available commands", this);
}
void CCommands::RegisterCommand(command *pCommand, const char *szName, const char *szDescription, CCommandHandler *pHandler)
{	pCommand->sName.Assign(szName);
    pCommand->sDescription.Assign(szDescription);
    pCommand->pHandler=pHandler;
    m_lCommands.push_back(pCommand);
}
command *CCommands::FindCommandByName(const char *szName, bool bExact)
{	list<command*>::iterator ic; // Cycle through commands and find the string
	for(ic=m_lCommands.begin(); ic!=m_lCommands.end(); ++ic)
        if(!(*ic)->sName.Compare(szName))
            return (*ic);
    return NULL;
}

bool CCommands::HandleCommand(CMessage *pMsg)
{
#ifdef DBGCONSOLE
        g_cMainCtrl.m_cConsDbg.Log(10, "File =%s:Line=%d:Function=%s \n",__FILE__, __LINE__, __FUNCTION__);
        g_cMainCtrl.m_cConsDbg.Log(10, "*%s* %s  \n", pMsg->sSrc.CStr(), pMsg->sChatString.CStr());
#endif
    if(!pMsg->sCmd.Compare("commands.list"))
	{	g_cMainCtrl.m_cIRC.SendMsg(pMsg->bSilent, pMsg->bNotice, "-[ command list ]-", pMsg->sReplyTo.Str());
		list<command*>::iterator ic; int iCount=0;
		for(ic=m_lCommands.begin(); ic!=m_lCommands.end(); ++ic)
		{	iCount++; CString sReply; sReply.Format("%d. / \"%s\" / \"%s\"", \
				iCount, (*ic)->sName.CStr(), (*ic)->sDescription.CStr());
			g_cMainCtrl.m_cIRC.SendMsg(pMsg->bSilent, pMsg->bNotice, sReply.Str(), pMsg->sReplyTo.Str());
			Sleep(1500); }
		return true; }

	return false; }
