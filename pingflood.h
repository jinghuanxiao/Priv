
#ifndef __PINGFLOOD_H__
#define __PINGFLOOD_H__

#include "cstring.h"
#include "ddos.h"

class CDDOSPingFlood : public CDDOSBase
{
public:
	CDDOSPingFlood();
	virtual void StartDDOS();
};

#endif // __PINGFLOOD_H__
