#include "stdafx.h"
#include "DecisionIndexWriter.h"
#include <vadefs.h>
#include <stdarg.h>
#include <stdio.h>

static const char *szXmlVersion = "1.0";
static const char* szFormatVersion = "1.0";

namespace DecisionIndex
{

	static void AddToElement(TiXmlElement **ppElement, ElementEntry *pEntries, int iNumEntries);
	static void AddElementId(TiXmlElement **ppElement, int id);


	DecisionIndexWriter::DecisionIndexWriter()
		: m_bOpen(false)
		, m_pDocument(NULL)
		, m_iId(0)
	{
	}

	DecisionIndexWriter::~DecisionIndexWriter()
	{
		Close();
	}

	bool DecisionIndexWriter::Open(const char *szOutputFilePath, const char *szFilename)
	{
		m_pDocument = new TiXmlDocument(szOutputFilePath);
		m_pDocument->LinkEndChild(new TiXmlDeclaration(szXmlVersion, "", ""));

		m_pMain_element = new TiXmlElement("DecisionIndex");
		m_pMain_element->SetAttribute("FormatVersion", szFormatVersion);
		m_pMain_element->SetAttribute("File", szOutputFilePath);

		m_ssFilename = szOutputFilePath;
		m_bOpen = true;

		m_iId = 0;

		return true;
	}

	std::string GetDecisionText(const TurnDirection & decision)
	{
		std::string str;
		switch (decision)
		{
		case(Direction_Left) :
			str = "L";
			break;
		case(Direction_Right) :
			str = "R";
			break;
		case(Direction_Unknown) :
			str = "U";
			break;
		default:
			break;
		}
		return str;
	}

	void AddToElement(TiXmlElement **ppElement, ElementEntry *pEntries, int iNumEntries)
	{
		for (int i = 0; i < iNumEntries; i++)
			(*ppElement)->SetAttribute(pEntries[i].first.c_str(), pEntries[i].second.c_str());
	}

	void AddElementId(TiXmlElement **ppElement, int id)
	{
		AddToElement(ppElement, XmlEntry("ID", "%i", id).GetData(), 1);
	}

	bool DecisionIndexWriter::AddIndex(DecisionIndexData const& data)
	{
		return AddIndex(data.direction, data.i64Time, data.dConfidence);
	}

	bool DecisionIndexWriter::AddIndex(const TurnDirection & decision, __int64 i64Time, double dConfidence
		, ElementEntry *pExtraInfo/* = NULL*/, int iNumExtraEntries/* = 0*/)
	{
		if (!m_bOpen)
			return false;

		TiXmlElement *direction_element = new TiXmlElement("Decision");

		AddElementId(&direction_element, m_iId++);
		AddToElement(&direction_element, XmlEntry("Type", GetDecisionText(decision)).GetData(), 1);
		AddToElement(&direction_element, XmlEntry("Time", "%I64d", i64Time).GetData(), 1);
		AddToElement(&direction_element, XmlEntry("Confidence", "%f", dConfidence).GetData(), 1);
		AddToElement(&direction_element, pExtraInfo, iNumExtraEntries);

		m_pMain_element->LinkEndChild(direction_element);

		direction_element->Clear();

		return true;
	}

	void DecisionIndexWriter::Close_Without_Saving()
	{
		if (m_pDocument)
			m_pDocument->Clear();

		m_ssFilename = "";
		m_bOpen = false;
		m_iId = 0;
	}

	void DecisionIndexWriter::Close()
	{
		if (m_bOpen)
			Save();

		Close_Without_Saving();
	}

	void DecisionIndexWriter::Save()
	{
		if (!m_bOpen)
			return;

		m_pDocument->LinkEndChild(m_pMain_element);
		m_pDocument->SaveFile(m_ssFilename.c_str());
		m_bOpen = false;
		Close();
		m_bOpen = false;
		m_iId = 0;
	}

	void DecisionIndexWriter::OnError(const char *szFmt, ...)
	{
		char szError[4096];
		va_list pArgs;
		va_start(pArgs, szFmt);
		_vsnprintf_s(szError, sizeof(szError), _TRUNCATE, szFmt, pArgs);
		sprintf_s(m_szLastError, sizeof(m_szLastError), "%s", szError);
	}

};
