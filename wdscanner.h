
#ifndef __WDSCANNER_H__
#define __WDSCANNER_H__

#include "cstring.h"
#include "scanner.h"

class CScannerWebDav : public CScannerBase
{
public:
	CScannerWebDav();
	virtual ~CScannerWebDav() throw() { }
	virtual void StartScan(const CString &sHost);
	virtual bool TestHost(const CString &sHost) { return true; }

	bool Exploit(const CString &sHost);
};

#endif // __WDSCANNER_H__
