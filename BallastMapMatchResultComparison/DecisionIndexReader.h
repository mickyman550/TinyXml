#pragma once

//#define TIXML_USE_STL

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>

class DecisionIndexReader
{
public:
	enum INDEX_TYPE{ LEFT, RIGHT, UNKNOWN };
	struct IndexData
	{
		IndexData()
		{
			index_type = INDEX_TYPE::LEFT;
			dNodeTime = 0.0;
		}

		IndexData(int id_in, INDEX_TYPE index_type_in, double dNodeTime_in)
			: id(id_in), index_type(index_type_in), dNodeTime(dNodeTime_in)
		{
		}
		int id;
		INDEX_TYPE index_type;
		double dNodeTime;
	};
public:
	DecisionIndexReader();
	~DecisionIndexReader();

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