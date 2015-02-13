// DecisionIndexConverter_Old_To_New.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DecisionIndexWriter.h"
#include "DirectionChangeIndexReader.h"

using namespace DecisionIndex;

TurnDirection GetOldIndexType(DirectionChangeIndexReader::INDEX_TYPE const & index_type)
{
	if (index_type == DirectionChangeIndexReader::RIGHT)
		return TurnDirection::Direction_Left;
	else if (index_type == DirectionChangeIndexReader::LEFT)
		return TurnDirection::Direction_Right;
	else return TurnDirection::Direction_Unknown;
}


int main(int argc, char **argv)
{

	const char *szFileIn = "C:\\Users\\develop\\201410300938.direction_3_reversed.xml";
	const char *szFileOut = "C:\\Users\\develop\\201410300938_NEW.direction_3.xml";

	DecisionIndex::Writer writer;
	writer.Open(szFileOut);


	DirectionChangeIndexReader reader_old;
	if (!reader_old.Load(szFileIn))
	{
		return 1;
	}

	for (int i = 0; i < reader_old.GetNumIndexes(); i++)
	{
		const DirectionChangeIndexReader::IndexData & data_in = reader_old.GetIndex(i);
		DecisionIndex::Data data = {};
		data.iId = i;
		data.dConfidence = 1.0;
		data.direction = GetOldIndexType(data_in.index_type);
		data.i64Time = data_in.i64StartTime + __int64(double(data_in.i64StartTime - data_in.i64EndTime) / 2.0);

		writer.AddIndex(data);
	}

	writer.Close();

	return 0;
}


