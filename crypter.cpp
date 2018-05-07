
#include "main.h"
#include "crypter.h"

CCryptStr::CCryptStr(int iKey) { m_iKey=iKey; m_szDecStr=NULL; }
CCryptStr::~CCryptStr() { Release(); }
void CCryptStr::Decrypt()
{	if(!m_iKey) return; if(m_szDecStr) free(m_szDecStr); m_szDecStr=(char*)malloc(GetLength());
	for(char i=0; i<GetLength(); i++) m_szDecStr[i]=m_szString[i]^(m_iKey+(i*(m_iKey%10)+1)); }
char *CCryptStr::Str() { Decrypt(); return m_szDecStr; }
void CCryptStr::Release() { if(m_szDecStr) { free(m_szDecStr); m_szDecStr=NULL; } }
