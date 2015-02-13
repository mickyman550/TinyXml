#include "stdafx.h"
#include "DecisionIndexWriter.h"
#include <vadefs.h>
#include <stdarg.h>
#include <stdio.h>

static const char *szXmlVersion = "1.0";
static const char* szFormatVersion = "1.0";

DecisionIndexWriter::DecisionIndexWriter()
: m_bOpen(false)
, m_pDoc(NULL)
, m_iId(0)
{
}

DecisionIndexWriter::~DecisionIndexWriter()
{
	Close();
}

bool DecisionIndexWriter::Open(const char *szOutputFilePath, DataType data_type, const char *szFilename)
{
	m_pDoc = new TiXmlDocument(szOutputFilePath);

	m_pDoc->LinkEndChild(new TiXmlDeclaration(szXmlVersion, "", ""));
	
	m_pDirectionIndex_element = new TiXmlElement("DirectionChangeIndex");
	m_pDirectionIndex_element->SetAttribute("FormatVersion", szFormatVersion);

	const char *szType;
	switch (data_type)
	{
	case(DataType::BPF) :
		szType = "bpf";
		break;
	case(DataType::SIX) :
		szType = "six";
		break;
	default:
		szType = "unknown";
		break;
	};
	m_pDirectionIndex_element->SetAttribute("Type", szType);
	m_pDirectionIndex_element->SetAttribute("File", szFilename);

	m_ssFilename = szOutputFilePath;
	m_bOpen = true;

	return true;
}

std::string DecisionIndexWriter::GetDecisionString(INDEX_TYPE index_type)
{
	const char *szType = "";
	switch(index_type)
	{
	case(INDEX_TYPE::LEFT) :
		szType = "Left";
		break;
	case(INDEX_TYPE::RIGHT) :
		szType = "Right";
		break;
	default:
		szType = "Unknown";
		break;
	}
	std::string str = szType;
	return str;
}

bool DecisionIndexWriter::AddIndex(INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime)
{
		if(!m_bOpen)
		return false;

	TiXmlElement *element;
	m_pDecision_element = new TiXmlElement("Decision");
	m_pDecision_element->SetAttribute("ID", m_iId);

	char szBuff[512];

	TiXmlElement *pMapmatch_element = new TiXmlElement("MapMatch");
	std::string ssMapMatchDecision = GetDecisionString(MapMatchDecisionType);
	pMapmatch_element->SetAttribute("Type", ssMapMatchDecision.c_str());
	sprintf_s(szBuff, sizeof(szBuff), "%f", dMapMatchNodeTime);
	pMapmatch_element->SetAttribute("NodeTime", szBuff);
	m_pDecision_element->LinkEndChild(pMapmatch_element);

	m_pDirectionIndex_element->LinkEndChild(m_pDecision_element);

	m_iId++;

	return true;
}

bool DecisionIndexWriter::AddIndex(int id, INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime, INDEX_TYPE BallastDecisionType, double dBallastTime, int iStartFrame, int iEndFrame)
{
	if(!m_bOpen)
		return false;

	TiXmlElement *element;
	m_pDecision_element = new TiXmlElement("Decision");
	m_pDecision_element->SetAttribute("ID", id);

	char szBuff[512];

	TiXmlElement *pMapmatch_element = new TiXmlElement("MapMatch");
	std::string ssMapMatchDecision = GetDecisionString(MapMatchDecisionType);
	pMapmatch_element->SetAttribute("Type", ssMapMatchDecision.c_str());
	sprintf_s(szBuff, sizeof(szBuff), "%f", dMapMatchNodeTime);
	pMapmatch_element->SetAttribute("NodeTime", szBuff);
	m_pDecision_element->LinkEndChild(pMapmatch_element);

	TiXmlElement *pBallast_element = new TiXmlElement("Ballast");
	std::string ssBallastDecision = GetDecisionString(BallastDecisionType);
	pBallast_element->SetAttribute("Type", ssBallastDecision.c_str());
	sprintf_s(szBuff, sizeof(szBuff), "%f", dBallastTime);
	pBallast_element->SetAttribute("Time", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%i", iStartFrame);
	pBallast_element->SetAttribute("StartFrame", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%i", iEndFrame);
	pBallast_element->SetAttribute("EndFrame", szBuff);
	m_pDecision_element->LinkEndChild(pBallast_element);

	m_pDirectionIndex_element->LinkEndChild(m_pDecision_element);

	m_iId++;

	return true;
}

bool DecisionIndexWriter::AddIndex(INDEX_TYPE MapMatchDecisionType, double dMapMatchNodeTime, double dMapMatchStartTime, double dMapMatchEndTime
								   , INDEX_TYPE BallastDecisionType, double dBallastTime, int iStartFrame, int iEndFrame)
{
	if(!m_bOpen)
		return false;

	TiXmlElement *element;
	m_pDecision_element = new TiXmlElement("Decision");
	m_pDecision_element->SetAttribute("ID", m_iId);

	char szBuff[512];

	TiXmlElement *pMapmatch_element = new TiXmlElement("MapMatch");
	std::string ssMapMatchDecision = GetDecisionString(MapMatchDecisionType);
	pMapmatch_element->SetAttribute("Type", ssMapMatchDecision.c_str());
	sprintf_s(szBuff, sizeof(szBuff), "%f", dMapMatchNodeTime);
	pMapmatch_element->SetAttribute("NodeTime", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%f", dMapMatchStartTime);
	pMapmatch_element->SetAttribute("StartTime", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%f", dMapMatchEndTime);
	pMapmatch_element->SetAttribute("EndTime", szBuff);
	m_pDecision_element->LinkEndChild(pMapmatch_element);

	TiXmlElement *pBallast_element = new TiXmlElement("Ballast");
	std::string ssBallastDecision = GetDecisionString(BallastDecisionType);
	pBallast_element->SetAttribute("Type", ssBallastDecision.c_str());
	sprintf_s(szBuff, sizeof(szBuff), "%f", dBallastTime);
	pBallast_element->SetAttribute("Time", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%i", iStartFrame);
	pBallast_element->SetAttribute("StartFrame", szBuff);
	sprintf_s(szBuff, sizeof(szBuff), "%i", iEndFrame);
	pBallast_element->SetAttribute("EndFrame", szBuff);
	m_pDecision_element->LinkEndChild(pBallast_element);

	m_pDirectionIndex_element->LinkEndChild(m_pDecision_element);

	m_iId++;

	return true;
}

void DecisionIndexWriter::Close()
{
	if (m_pDoc && m_bOpen)
		m_pDoc->Clear();
	m_pDoc = NULL;
	m_iId=0;
}

void DecisionIndexWriter::Save()
{
	if(!m_bOpen)
		return;

	m_pDoc->LinkEndChild(m_pDirectionIndex_element);
	m_pDoc->SaveFile(m_ssFilename.c_str());
	Close();
	m_bOpen = false;
	m_iId=0;
}

void DecisionIndexWriter::OnError(const char *szFmt,...)
{
	char szError[4096];
	va_list pArgs;
	va_start(pArgs, szFmt);
	_vsnprintf_s(szError, sizeof(szError), _TRUNCATE, szFmt, pArgs);
	sprintf_s( m_szLastError, sizeof( m_szLastError ), "%s", szError );

}