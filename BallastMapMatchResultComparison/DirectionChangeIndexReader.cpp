#include "stdafx.h"
#include "DirectionChangeIndexReader.h"
#include <stdarg.h>
#include <assert.h>

DirectionChangeIndexReader::DirectionChangeIndexReader()
{
}

DirectionChangeIndexReader::~DirectionChangeIndexReader()
{
}


bool DirectionChangeIndexReader::Load(const char *szFilename)
{
	m_vIndexData.clear();

	TiXmlDocument doc;
	if(!doc.LoadFile(szFilename))
	{
		OnError(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if(root == NULL)
	{
		OnError("Failed to load file: No root element.");
		doc.Clear();
		return false;
	}

	const char* attr;
	__int64 i64StartTime = 0;
	__int64 i64EndTime = 0;
	int iStartFrame = 0;
	int iEndFrame = 0;
	INDEX_TYPE index_type;
	for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string ssElemName = elem->Value();
		std::string ssIndexType = elem->Attribute("Type");
		if( ssIndexType == "Left")
			index_type = INDEX_TYPE::LEFT;
		else if(ssIndexType == "Right")
			index_type = INDEX_TYPE::RIGHT;
		else
			index_type = INDEX_TYPE::UNKNOWN;	//should never happen!

		for(TiXmlElement* ChildElement = elem->FirstChildElement(); ChildElement != NULL; ChildElement = ChildElement->NextSiblingElement())
		{
			std::string ssChildElemName = ChildElement->Value();

			if(ssChildElemName == "Time")
			{
				i64StartTime = _atoi64(ChildElement->Attribute("Start"));
				i64EndTime = _atoi64(ChildElement->Attribute("End"));
			}
			else if( ssChildElemName == "Frame" )
			{
				iStartFrame = atoi(ChildElement->Attribute("Start"));
				iEndFrame = atoi(ChildElement->Attribute("End"));
			}
			else
			{
				//add other conditions if more element types are needed.
				continue;
			}
		}
		m_vIndexData.push_back(IndexData(index_type, i64StartTime, i64EndTime, iStartFrame, iEndFrame));
	}

	doc.Clear();

	return true;
}

DirectionChangeIndexReader::IndexData const& DirectionChangeIndexReader::GetIndex(int i)
{
	assert( i >= 0 && i < m_vIndexData.size() );
	return m_vIndexData[i];
}

int DirectionChangeIndexReader::GetNumIndexes()
{
	return m_vIndexData.size();
}

void DirectionChangeIndexReader::OnError(const char *szFmt, ...)
{
	va_list pArgs;
	va_start(pArgs, szFmt);
	_vsnprintf_s(m_szLastError, sizeof(m_szLastError), _TRUNCATE, szFmt, pArgs);
}


