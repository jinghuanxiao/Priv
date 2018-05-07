
#ifndef __SYNFLOOD_H__
#define __SYNFLOOD_H__

#include "cstring.h"
#include "ddos.h"
#include "utility.h"

class CDDOSSynFlood : public CDDOSBase
{
public:
	CDDOSSynFlood();
	virtual void StartDDOS();
};

#endif // __SYNFLOOD_H__
