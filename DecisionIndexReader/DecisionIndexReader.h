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
***************************************************************************************************

*/

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>

#include "DecisionIndexData.h"

namespace DecisionIndex
{

	class DecisionIndexReader
	{
	public:
		DecisionIndexReader();
		~DecisionIndexReader();

		void Clear();

		bool Load(const char *szFilename);

		int GetNumIndexes();
		DecisionIndexData const& GetIndex(int i);
		const char *GetLastError(){ return m_szLastError; }
	private:
		void OnError(const char *szFmt, ...);
	private:
		std::vector<DecisionIndexData> m_vIndexData;
		char m_szLastError[2048];
	};

};