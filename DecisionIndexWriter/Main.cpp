// DecisionIndexReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DecisionIndexWriter.h"

using namespace DecisionIndex;

int main(int argc, char** argv)
{

	//if(argc != 2)
	//	return 1;

	const char *szFile = "C:\\develop\\801 - Map\\DecisionIndex\\test.decision.xml";//argv[1];//

	DecisionIndexWriter writer;
	if(!writer.Open("C:\\develop\\801 - Map\\DecisionIndex\\test.decision.xml"))
	{
		return 1;
	}

	__int64 i64Time = 324598745969800;
	for (int i = 0; i < 5; i++)
	{
		if (!writer.AddIndex(TurnDirection::Direction_Left, i64Time+i*1000, 1.0, NULL, 0))
		{
			return 1;
		}
	}

	writer.Save();
	writer.Close();

	return 0;
}

