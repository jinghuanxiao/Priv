
#ifndef __DCOMSCANNER_H__
#define __DCOMSCANNER_H__

#include "cstring.h"
#include "scanner.h"

class CScannerDCOM : public CScannerBase
{
public:
	CScannerDCOM();
	virtual ~CScannerDCOM() throw() { }
	virtual void StartScan(const CString &sHost);
	virtual bool TestHost(const CString &sHost) { return true; }

	bool Exploit(const CString &sHost);
	bool ExploitNP(const CString &sHost);
};

#endif // __DCOMSCANNER_H__
