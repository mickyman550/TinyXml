// DecisionIndexReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DecisionIndexReader.h"

using namespace DecisionIndex;

int main(int argc, char** argv)
{

	//if(argc != 2)
	//	return 1;

	const char *szFile = "C:\\develop\\801 - Map\\DecisionIndex\\test.decision.xml";//argv[1];//

	DecisionIndexReader reader;

	if(!reader.Load(szFile))
	{
		return 1;
	}

	std::vector<DecisionIndex::DecisionIndexData> vDecisionIndexData(reader.GetNumIndexes());
	for( int i = 0; i < reader.GetNumIndexes(); i++)
	{
		vDecisionIndexData[i] = reader.GetIndex(i);
	}

	return 0;
}

