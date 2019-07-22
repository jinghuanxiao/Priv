
#ifndef __HTTPFLOOD_H__
#define __HTTPFLOOD_H__

#include "cstring.h"
#include "ddos.h"

class CDDOSHTTPFlood : public CDDOSBase
{
public:
	CDDOSHTTPFlood();
	virtual void StartDDOS();
};

#endif // __HTTPFLOOD_H__
