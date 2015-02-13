#pragma once

//#define TIXML_USE_STL

#include "tinyxml.h"
#include "tinystr.h"
#include <string>

class DecisionIndexWriter
{
public:
	enum INDEX_TYPE{ LEFT, RIGHT, UNKNOWN };
	enum DataType{ BPF = 1, SIX = 2 };
public:
	DecisionIndexWriter();
	~DecisionIndexWriter();

	bool Open(const char *szOutputFilePath, DataType data_type, const char *szFilename = "");

	bool AddIndex(int id, INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime, INDEX_TYPE BallastDecisionType, double dBallastTime, int iStartFrame, int iEndFrame);

	bool AddIndex(INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime, double dMapMatchStartTime, double dMapMatchEndTime
		, INDEX_TYPE BallastDecisionType, double dBallastTime, int iStartFrame, int iEndFrame);

	bool AddIndex(INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime);

	int GetId(){ return m_iId; } 

	void Save();

	void Close();

	bool IsOpen(){ return m_bOpen; }
	const char *GetLastError(){	return m_szLastError; }
private:
	void OnError(const char *szFmt, ...);
	std::string GetDecisionString(INDEX_TYPE index_type);
private:
	TiXmlDocument *m_pDoc;
	TiXmlElement *m_pDirectionIndex_element;
	TiXmlElement *m_pDecision_element;
private:
	int m_iId;
	bool m_bOpen;
	std::string m_ssFilename;
	char m_szLastError[512];
};

