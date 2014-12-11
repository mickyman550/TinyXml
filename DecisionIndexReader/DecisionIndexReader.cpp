#include "stdafx.h"
#include "DecisionIndexReader.h"
#include <stdarg.h>
#include <assert.h>

namespace DecisionIndex
{

	DecisionIndexReader::DecisionIndexReader()
	{
	}

	DecisionIndexReader::~DecisionIndexReader()
	{
		Clear();
	}

	//in case any tinyxml stuff needs to be cleared.
	void DecisionIndexReader::Clear()
	{
		m_vIndexData.clear();
	}

	TurnDirection GetDecisionType(const char *szType)
	{
		TurnDirection index_type;

		std::string ssType = szType;

		if (ssType == "L")
			index_type = Direction_Left;
		else if (ssType == "R")
			index_type = Direction_Right;
		else
			index_type = Direction_Unknown;

		return index_type;
	}

	bool DecisionIndexReader::Load(const char *szFilename)
	{
		Clear();

		TiXmlDocument doc;
		if (!doc.LoadFile(szFilename))
		{
			OnError(doc.ErrorDesc());
			return false;
		}

		TiXmlElement* root = doc.FirstChildElement();
		if (root == NULL)
		{
			OnError("Failed to load file: No root element.");
			doc.Clear();
			return false;
		}

		const char* attr;
		TurnDirection index_type;
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string ssElemName = elem->Value();
			if (ssElemName != "Decision")
				continue;

			std::string ssId = elem->Attribute("ID");
			std::string ssType = elem->Attribute("Type");
			std::string ssTime = elem->Attribute("Time");
			std::string ssConfidence = elem->Attribute("Confidence");

			if (ssType == "L")
				index_type = Direction_Left;
			else if (ssType == "R")
				index_type = Direction_Right;
			else
				index_type = Direction_Unknown;

			int iId = atoi(ssId.c_str());
			__int64 i64Time = _atoi64(ssTime.c_str());
			double dConfidence = atof(ssConfidence.c_str());
			m_vIndexData.push_back(DecisionIndexData(iId, GetDecisionType(ssType.c_str()), i64Time, dConfidence));
		}

		doc.Clear();

		return true;
	}

	DecisionIndexData const& DecisionIndexReader::GetIndex(int i)
	{
		assert(i >= 0 && i < m_vIndexData.size());
		return m_vIndexData[i];
	}

	int DecisionIndexReader::GetNumIndexes()
	{
		return m_vIndexData.size();
	}

	void DecisionIndexReader::OnError(const char *szFmt, ...)
	{
		va_list pArgs;
		va_start(pArgs, szFmt);
		_vsnprintf_s(m_szLastError, sizeof(m_szLastError), _TRUNCATE, szFmt, pArgs);
	}

};
