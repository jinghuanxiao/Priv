
#ifndef __UDPFLOOD_H__
#define __UDPFLOOD_H__

#include "cstring.h"
#include "ddos.h"

class CDDOSUDPFlood : public CDDOSBase
{
public:
	CDDOSUDPFlood();
	virtual void StartDDOS();
};

#endif // __UDPFLOOD_H__
