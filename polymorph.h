
#ifndef __POLYMORPH_H__
#define __POLYMORPH_H__

#ifdef WIN32
class CPolymorph
{
public:
	CPolymorph();
	CPolymorph(const char *szFile);
	~CPolymorph();

	bool MapFile(const char *szFile, char **szBuffer);
	void UnmapFile(char **szBuffer);

	bool DoPolymorph(const char *szFile);
};
#endif // WIN32

#endif // __POLYMORPH_H__
