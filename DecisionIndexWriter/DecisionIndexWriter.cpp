#include "stdafx.h"
#include "DecisionIndexWriter.h"
#include <vadefs.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

static const char *szXmlVersion = "1.0";
static const char* szFormatVersion = "1.0";

namespace DecisionIndex
{

	static void AddToElement(TiXmlElement **ppElement, ElementEntry *pEntries, int iNumEntries);
	static void AddElementId(TiXmlElement **ppElement, int id);


	class XmlEntry
	{
	public:
		XmlEntry(const char *szFirst, std::string const&  str)
		{
			m_pElementEntry = new ElementEntry(szFirst, str.c_str());
		}

		XmlEntry(const char *szFirst, const char *szSecond, ...)
			: m_pElementEntry(0)
		{
			char szBuff[128];
			va_list pArgs;
			va_start(pArgs, szSecond);
			_vsnprintf_s(szBuff, sizeof(szBuff), _TRUNCATE, szSecond, pArgs);
			m_pElementEntry = new ElementEntry(szFirst, szBuff);
		}

		~XmlEntry()
		{
			if (m_pElementEntry)
				delete m_pElementEntry;
		}

		ElementEntry * GetData()
		{
			assert(m_pElementEntry);
			return m_pElementEntry;
		}
	private:
		ElementEntry *m_pElementEntry;
	};


	// Change the file extension part of this filename. Pass in the new extension without the '.'
	static std::string changeFileExtension(const char * szName, const char * szExt)
	{
		std::string ssName = szName;
		std::string::size_type pos = ssName.rfind('.');
		while (pos != std::string::npos)
		{
			ssName.erase(pos);
			pos = ssName.rfind('.');
		}
		ssName += '.';
		ssName += szExt;
		return ssName;
	}

	std::string getFilenameFromPath(const char *szFilePath)
	{
		std::string ssFilePath = szFilePath;
		std::string ssFileName;

		size_t find = ssFilePath.find_last_of("\\");
		if (find == std::string::npos)	//not a path.
			return szFilePath;
		else if (find == ssFilePath.length())	//directory
			return szFilePath;
		else
			ssFileName = ssFilePath.substr(find + 1, ssFilePath.length());	//filename

		return ssFileName;
	}

	Writer::Writer()
		: m_bOpen(false)
		, m_pDocument(NULL)
		, m_iId(0)
	{
	}

	Writer::~Writer()
	{
		Close();
	}

	bool Writer::Open(const char *szOutputFilePath)
	{
		m_ssFilepath = changeFileExtension(szOutputFilePath, "decision.xml");
		m_pDocument = new TiXmlDocument(m_ssFilepath.c_str());
		m_pDocument->LinkEndChild(new TiXmlDeclaration(szXmlVersion, "", ""));

		std::string ssFilename = getFilenameFromPath(m_ssFilepath.c_str());
		m_pMain_element = new TiXmlElement("DecisionIndex");
		m_pMain_element->SetAttribute("FormatVersion", szFormatVersion);
		m_pMain_element->SetAttribute("File", ssFilename.c_str());

		m_bOpen = true;
		m_iId = 0;
		return true;
	}

	std::string GetDecisionText(const TurnDirection & decision)
	{
		if (decision == Direction_Left)
			return "L";
		else if (decision == Direction_Right)
			return "R";
		else
			return "U";
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

	bool Writer::AddIndex(DecisionIndex::Data const& data)
	{
		return AddIndex(data.direction, data.i64Time, data.dConfidence);
	}

	bool Writer::AddIndex(const TurnDirection & decision, __int64 i64Time, double dConfidence
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

	void Writer::Close_Without_Saving()
	{
		if (m_pDocument)
			m_pDocument->Clear();

		m_ssFilepath = "";
		m_bOpen = false;
		m_iId = 0;
	}

	void Writer::Close()
	{
		if (m_bOpen)
			Save();

		Close_Without_Saving();
	}

	void Writer::Save()
	{
		if (!m_bOpen)
			return;

		m_pDocument->LinkEndChild(m_pMain_element);
		m_pDocument->SaveFile(m_ssFilepath.c_str());
		m_bOpen = false;
		Close();
		m_bOpen = false;
		m_iId = 0;
	}

	void Writer::OnError(const char *szFmt, ...)
	{
		char szError[4096];
		va_list pArgs;
		va_start(pArgs, szFmt);
		_vsnprintf_s(szError, sizeof(szError), _TRUNCATE, szFmt, pArgs);
		sprintf_s(m_szLastError, sizeof(m_szLastError), "%s", szError);
	}

};
