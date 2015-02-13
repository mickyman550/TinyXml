#include "stdafx.h"
#include "DecisionIndexReader.h"
#include <stdarg.h>
#include <assert.h>

DecisionIndexReader::DecisionIndexReader()
{
}

DecisionIndexReader::~DecisionIndexReader()
{
}


bool DecisionIndexReader::Load(const char *szFilename)
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
	INDEX_TYPE index_type;
	double dNodeTime = 0.0;
	for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string ssElemName = elem->Value();
if(ssElemName=="State")
   continue;
		int id = atoi(elem->Attribute("ID"));
		std::string ssIndexType;
		for(TiXmlElement* ChildElement = elem->FirstChildElement(); ChildElement != NULL; ChildElement = ChildElement->NextSiblingElement())
		{
			std::string ssChildElemName = ChildElement->Value();

			if(ssChildElemName == "MapMatch")
			{
				
				ssIndexType = ChildElement->Attribute("Type");
				if( ssIndexType == "Left")
					index_type = INDEX_TYPE::LEFT;
				else if(ssIndexType == "Right")
					index_type = INDEX_TYPE::RIGHT;
				else
					index_type = INDEX_TYPE::UNKNOWN;	//should never happen!

				dNodeTime = atof(ChildElement->Attribute("NodeTime"));
			}
			else
			{
				//add other conditions if more element types are needed.
				continue;
			}
		}
		m_vIndexData.push_back(IndexData(id,index_type, dNodeTime));
	}

	doc.Clear();

	return true;
}

DecisionIndexReader::IndexData const& DecisionIndexReader::GetIndex(int i)
{
	assert( i >= 0 && i < m_vIndexData.size() );
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


