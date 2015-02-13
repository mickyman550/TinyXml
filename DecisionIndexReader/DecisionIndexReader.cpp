#include "stdafx.h"
#include "DecisionIndexReader.h"
#include <stdarg.h>
#include <assert.h>

namespace DecisionIndex
{

	static bool SetOrder(const DecisionIndex::Data & lhs, const DecisionIndex::Data & rhs)
	{
		return lhs.i64Time > rhs.i64Time;
	}

	Reader::Reader()
	{
		m_psetIndexData = new OrderedSet(SetOrder);
	}

	Reader::~Reader()
	{
		Clear();
	}

	void Reader::Clear()
	{
		if(m_psetIndexData)
		{
			m_psetIndexData->clear();
			delete m_psetIndexData;
			m_psetIndexData = 0;
		}
	}

	TurnDirection GetDecisionType(const char *szType)
	{
		std::string ssType = szType;
		if (ssType == "L")
			return Direction_Left;
		else if (ssType == "R")
			return Direction_Right;
		else
			return Direction_Unknown;
	}

	bool Reader::Load(const char *szFilename)
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
			addToBuffer(DecisionIndex::Data(iId, GetDecisionType(ssType.c_str()), i64Time, dConfidence));
		}

		doc.Clear();

		return true;
	}

	void Reader::addToBuffer(DecisionIndex::Data const& data)
	{
		if(!m_psetIndexData)
			m_psetIndexData = new OrderedSet(SetOrder);

		m_psetIndexData->insert(data);
	}

	bool Reader::GetDecision(DecisionIndex::Data & data)
	{
		if(!m_psetIndexData->size())
			return false;

		data = *m_psetIndexData->rbegin();
		m_psetIndexData->erase(data);
		return true;
	}

	void Reader::OnError(const char *szFmt, ...)
	{
		va_list pArgs;
		va_start(pArgs, szFmt);
		_vsnprintf_s(m_szLastError, sizeof(m_szLastError), _TRUNCATE, szFmt, pArgs);
	}

};
