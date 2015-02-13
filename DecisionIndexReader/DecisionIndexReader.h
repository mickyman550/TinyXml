#pragma once

/*
Class for reading a xml file written by DecisionIndexWriter.
Fields include: LEFT/RIGHT/UNKNOWN decisions,time, and a confidence value.
example row: <Decision ID="5" Type="R" Time="635502607413618158" Confidence="1.000000" />
***************************************************************************************************
Authors :
Michael McGlynn (MRM)
***************************************************************************************************
Ver: By:  Date:       Comment:
1.0  MRM  11/12/2014  First version
1.1  MRM  12/12/2014  Use ordered set instead of vector
***************************************************************************************************

*/

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>
#include <set>
#include "DecisionIndexData.h"

namespace DecisionIndex
{

	//could be used to get a decision from any given context.
	class IReader
	{
	public:
		virtual bool GetDecision(DecisionIndex::Data & data) = 0;
	};

	typedef std::set<DecisionIndex::Data, bool(*)( const DecisionIndex::Data &, const DecisionIndex::Data &)> OrderedSet;

	//read decisions from a file.
	class Reader : public IReader
	{
	public:
		Reader();
		~Reader();

		void Clear();

		bool Load(const char *szFilename);

		virtual bool GetDecision(DecisionIndex::Data & data);

		const char *GetLastError(){ return m_szLastError; }
	private:
		void addToBuffer(const DecisionIndex::Data & data);
		void OnError(const char *szFmt, ...);
	private:
		OrderedSet *m_psetIndexData;
		char m_szLastError[2048];
	};

};