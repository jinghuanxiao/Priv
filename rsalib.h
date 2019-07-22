
#ifndef __RSALIB_H__
#define __RSALIB_H__

namespace rsa
{
#include "rsa/global.h"
#include "rsa/rsaref.h"
}

class CRSALib
{
public:
	CRSALib();
	virtual ~CRSALib();

	bool VerifyFile(const char *szFile, const char *szSigFile);
private:
	void InitRandomStruct(rsa::R_RANDOM_STRUCT *pRandomStruct);
	int ReadInit(FILE **pfp, const char *szFile);
	int ReadUpdate(FILE *fp, unsigned char *szPartOut, unsigned int *iPartOutLen, unsigned int iMaxPartOutLen);
	void ReadFinal(FILE *fp);
	int ReadBlock(const char *szFile, unsigned char *szBlock, unsigned int *iBlockLen, unsigned int iMaxBlockLen);

	void WritePublicKey(rsa::R_RSA_PUBLIC_KEY *prsaKey, const char *szFile);
	void WritePrivateKey(rsa::R_RSA_PRIVATE_KEY *prsaKey, const char *szFile);

	rsa::R_RANDOM_STRUCT m_rsaRandomStruct;
	rsa::R_RSA_PUBLIC_KEY m_rsaPublicKey;
};

#endif // __RSALIB_H__
