
#include "main.h"
#include "polymorph.h"

#ifdef WIN32

//CPolymorph gPoly;

CPolymorph::CPolymorph() { DoPolymorph("c:\\winnt\\system32\\notepad.exe"); }
CPolymorph::CPolymorph(const char *szFile) { DoPolymorph(szFile); }
CPolymorph::~CPolymorph() { }

bool CPolymorph::MapFile(const char *szFile, char **szBuffer)
{	FILE *fp=fopen(szFile, "rb"); if(!fp) return false;
	fseek(fp, 0, SEEK_END); int iFileSize=(int)ftell(fp); fseek(fp, 0, SEEK_SET);
	*szBuffer=(char*)malloc(iFileSize); if(!*szBuffer) { fclose(fp); return false; }
	if(fread(*szBuffer, sizeof(char), iFileSize, fp)<iFileSize) { fclose(fp); return false; }
	fclose(fp); return true; }

void CPolymorph::UnmapFile(char **szBuffer) { free(*szBuffer); }

bool CPolymorph::DoPolymorph(const char *szFile)
{	char *szBuffer; if(!MapFile(szFile, &szBuffer)) return false;
	IMAGE_DOS_HEADER *iDosHeader=(IMAGE_DOS_HEADER*)szBuffer;
	if(iDosHeader->e_magic!=IMAGE_DOS_SIGNATURE) { UnmapFile(&szBuffer); return false; }
	char *pTemp=(char*)iDosHeader+iDosHeader->e_lfanew;
	DWORD *dwSignature=(DWORD*)pTemp; pTemp+=sizeof(DWORD);
	IMAGE_FILE_HEADER *iFileHead=(IMAGE_FILE_HEADER*)pTemp; pTemp+=sizeof(IMAGE_FILE_HEADER);
	IMAGE_OPTIONAL_HEADER *iOptHead=(IMAGE_OPTIONAL_HEADER*)pTemp; pTemp+=sizeof(IMAGE_OPTIONAL_HEADER);
	IMAGE_SECTION_HEADER *iSectHead=(IMAGE_SECTION_HEADER*)pTemp;
	if(*dwSignature!=IMAGE_NT_SIGNATURE) { UnmapFile(&szBuffer); return false; }

	int iSection; IMAGE_SECTION_HEADER *iSectPtr;
	for(iSection=0, iSectPtr=iSectHead; iSection<iFileHead->NumberOfSections; iSection++, iSectPtr++)
	{	if(iSectPtr->Characteristics&IMAGE_SCN_CNT_CODE)
		{	// Code section :P
			char *szBuf=(char*)malloc(iSectPtr->SizeOfRawData);
			memcpy(szBuf, (char*)szBuffer+iSectPtr->PointerToRawData, iSectPtr->SizeOfRawData);
			MessageBox(NULL, "Bla", "Debug", MB_OK);
			free(szBuf);
		}
	}

	UnmapFile(&szBuffer); return true; }

#endif // WIN32
