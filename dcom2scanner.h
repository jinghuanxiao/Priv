
#ifndef __DCOM2SCANNER_H__
#define __DCOM2SCANNER_H__

#include "cstring.h"
#include "scanner.h"

class CScannerDCOM2 : public CScannerBase
{
public:
	CScannerDCOM2();
	virtual ~CScannerDCOM2() throw() { }
	virtual void StartScan(const CString &sHost);
	virtual bool TestHost(const CString &sHost) { return true; }

	bool Exploit(const CString &sHost);
	bool ExploitNP(const CString &sHost);
};

#endif // __DCOM2SCANNER_H__
