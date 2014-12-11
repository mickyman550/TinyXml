#pragma once

//#define TIXML_USE_STL

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>

class DirectionChangeIndexReader
{
public:
	enum INDEX_TYPE{ LEFT, RIGHT, UNKNOWN };
	struct IndexData
	{
		IndexData()
		{
			index_type = INDEX_TYPE::LEFT;
			i64StartTime = 0;
			i64EndTime = 0;
			iStartFrame = 0;
			iEndFrame = 0;
		}

		IndexData(INDEX_TYPE index_type_in, __int64 i64StartTime_in, __int64 i64EndTime_in, int iStartFrame_in, int iEndFrame_in)
			: index_type(index_type_in), i64StartTime(i64StartTime_in), i64EndTime(i64EndTime_in), iStartFrame(iStartFrame_in), iEndFrame(iEndFrame_in)
		{
		}
		INDEX_TYPE index_type;
		__int64 i64StartTime;
		__int64 i64EndTime;
		int iStartFrame;
		int iEndFrame;
	};
public:
	DirectionChangeIndexReader();
	~DirectionChangeIndexReader();

	bool Load(const char *szFilename);

	int GetNumIndexes();
	IndexData const& GetIndex(int i);

	const char *GetLastError(){ return m_szLastError; }
private:
	std::vector<IndexData> m_vIndexData;
	void OnError(const char *szFmt, ...);
private:
	char m_szLastError[2048];
};