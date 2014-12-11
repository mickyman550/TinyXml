#pragma once

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