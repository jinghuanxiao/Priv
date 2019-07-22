
#ifndef __JUNOFLOOD_H__
#define __JUNOFLOOD_H__

#include "cstring.h"
#include "ddos.h"

class CDDOSJunoFlood : public CDDOSBase
{
public:
	CDDOSJunoFlood();
	virtual void StartDDOS();
};

#endif // __JUNOFLOOD_H__
