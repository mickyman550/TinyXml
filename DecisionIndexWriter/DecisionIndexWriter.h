#pragma once

/*
Class for writing an xml with LEFT/RIGHT/UNKNOWN decisions indexed against time, with a confidence value.
example row: <Decision ID="5" Type="R" Time="635502607413618158" Confidence="1.000000" />
Optional elements can be added for debugging.
***************************************************************************************************
Authors :
Michael McGlynn (MRM)
***************************************************************************************************
Ver: By:  Date:       Comment:
1.0  MRM  11/12/2014  First version
***************************************************************************************************
*/

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <stdarg.h>

#include "DecisionIndexData.h"

namespace DecisionIndex
{

	typedef std::pair<std::string, std::string> ElementEntry;

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

	class DecisionIndexWriter
	{
	public:
		DecisionIndexWriter();
		~DecisionIndexWriter();

		bool Open(const char *szOutputFilePath, const char *szFilename = "");

		bool AddIndex(const TurnDirection & index_type, __int64 i64Time, double dConfidence, ElementEntry *pExtraInfo = NULL, int iNumExtraEntries = 0);
		bool AddIndex(DecisionIndexData const& data);

		void Save();

		//also saves the file.
		void Close();

		void Close_Without_Saving();

		bool IsOpen(){ return m_bOpen; }
		const char *GetLastError(){ return m_szLastError; }
	private:
		void OnError(const char *szFmt, ...);
	private:
		TiXmlDocument *m_pDocument;
		TiXmlElement *m_pMain_element;
	private:
		bool m_bOpen;
		int m_iId;
		std::string m_ssFilename;
		char m_szLastError[512];
	};

	//
	//
	//typedef std::pair<std::string,std::string> ElementEntry;
	//
	////class ElementEntry : public pair_of_strings
	////{
	////public:
	////	ElementEntry()
	////		: m_pData(0)
	////	{
	////	}
	////
	////	~ElementEntry()
	////	{
	////		if(m_pData)
	////			delete m_pData;
	////	}
	////
	////	ElementEntry(const char * str1, const char * str2)
	////	{
	////		m_pData = new pair_of_strings(str1, str2);
	////	}
	////
	////	ElementEntry(std::string & str1, std::string & str2)
	////	{
	////		m_pData = new pair_of_strings(str1, str2);
	////	}
	////
	////	ElementEntry(pair_of_strings &data)
	////	{
	////		m_pData = new pair_of_strings(data);
	////	}
	////
	////	std::pair<std::string, std::string> & operator= (const ElementEntry & other)
	////	{
	////		return *m_pData;
	////	}
	////
	////	//ElementEntry * operator& (const ElementEntry * pOther)
	////	//{
	////	//	return m_pElementEntry;
	////	//}
	////
	////	//ElementEntry * operator* (const ElementEntry * pOther)
	////	//{
	////	//	return m_pElementEntry;
	////	//}
	////
	////	//ElementEntry * operator* (const ElementEntry & other)
	////	//{
	////	//	return m_pElementEntry;
	////	//}
	////
	////	pair_of_strings * operator& (const ElementEntry & other)
	////	{
	////		return m_pData;
	////	}
	////
	////	//ElementEntry * operator= (const ElementEntry * other)
	////	//{
	////	//	return m_pElementEntry;
	////	//}
	////
	////	pair_of_strings & operator= (const pair_of_strings & other)
	////	{
	////		return *m_pData;
	////	}
	////
	////	std::string first;
	////	std::string second;
	////protected:
	////	pair_of_strings *m_pData;
	////};
	//
	//class XmlEntry/* : public ElementEntry*/
	//{
	//public:
	//	XmlEntry(const char *szFirst, const char *szSecond, ...)
	//		: m_pElementEntry(0)
	//	{
	//		char szBuff[128];
	//		va_list pArgs;
	//		va_start(pArgs, szSecond);
	//		_vsnprintf_s(szBuff, sizeof(szBuff), _TRUNCATE, szSecond, pArgs);
	//		m_pElementEntry = new ElementEntry(szFirst, szBuff);
	//	}
	//
	//	~XmlEntry()
	//	{
	//		if(m_pElementEntry)
	//			delete m_pElementEntry;
	//	}
	//
	//	ElementEntry * GetData()
	//	{ 
	//		assert(m_pElementEntry);
	//		return m_pElementEntry; 
	//	}
	//
	//	//ElementEntry * operator& (const ElementEntry * pOther)
	//	//{
	//	//	return m_pElementEntry;
	//	//}
	//
	//	//ElementEntry * operator* (const ElementEntry * pOther)
	//	//{
	//	//	return m_pElementEntry;
	//	//}
	//
	//	//ElementEntry * operator* (const ElementEntry & other)
	//	//{
	//	//	return m_pElementEntry;
	//	//}
	//
	//	//ElementEntry * operator& (const ElementEntry & other)
	//	//{
	//	//	return m_pElementEntry;
	//	//}
	//
	//	//ElementEntry * operator= (const ElementEntry * other)
	//	//{
	//	//	return m_pElementEntry;
	//	//}
	//
	//	//ElementEntry & operator= (const ElementEntry & other)
	//	//{
	//	//	return *m_pElementEntry;
	//	//}
	//private:
	//	ElementEntry *m_pElementEntry;
	//};

};
