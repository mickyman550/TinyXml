// BallastMapMatchResultComparison.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DirectionChangeIndexReader.h"
#include "DecisionIndexReader.h"
#include "DecisionIndexWriter.h"
#include "TimeCodeConverter.h"
#include <assert.h>

bool GetClosestBallastDecision( DirectionChangeIndexReader &ballast_reader, double dTime, DecisionIndexReader::INDEX_TYPE decision, double *pdTimeDiff, int *piMinIndex )
{
	double dMinTimeDiff = 0.0;
	int iMin = -1;
	double dBallastSeconds = 0.0;
	for( int i = 0; i < ballast_reader.GetNumIndexes();i++)
	{
		DirectionChangeIndexReader::IndexData const& index = ballast_reader.GetIndex(i);
		double dSeconds = 0.0;
		TimeCodeConverter::TicksToSecondsOfWeek(_abs64(index.i64StartTime), &dSeconds);
		double dT = fabs(dTime - dSeconds);
		if(decision == index.index_type && (dT < dMinTimeDiff || iMin < 0))
		{
			dMinTimeDiff = dT;
			iMin = i;
		}
	}
	if(iMin < 0)
		return false;	//unable to find match.
	*pdTimeDiff = dMinTimeDiff;
	*piMinIndex = iMin;
	return true;
}

int main(int argc, char** argv)
{
	if(argc != 4)
		return 1;

	const char *szBallastFile = argv[1];
	const char *szMapMatchFile = argv[2];
	const char *szFileOut = argv[3];

	DirectionChangeIndexReader ballast_reader;
	if(!ballast_reader.Load(szBallastFile))
	{
		return 1;
	}

	DecisionIndexReader mapmatch_reader;
	if(!mapmatch_reader.Load(szMapMatchFile))
	{
		return 1;
	}

	DecisionIndexWriter writer;
	if(!writer.Open(szFileOut, (DecisionIndexWriter::DataType)NULL))
	{
		return 1;
	}


	double dMaxTimeDiff = 100.0;
	for( int i = 0; i < mapmatch_reader.GetNumIndexes();i++)
	{
		DecisionIndexReader::IndexData const& index = mapmatch_reader.GetIndex(i);
		double dMinTimeDiff = 0.0;
		int iMinIndex = 0;

		if(!GetClosestBallastDecision(ballast_reader, index.dNodeTime, index.index_type, &dMinTimeDiff, &iMinIndex))
			continue;

		if(dMinTimeDiff > dMaxTimeDiff)
		{
			assert(iMinIndex >= 0 && iMinIndex < ballast_reader.GetNumIndexes());
			DirectionChangeIndexReader::IndexData const& ballast_index = ballast_reader.GetIndex(iMinIndex);


			double dBallastTime = 0.0;
			TimeCodeConverter::TicksToSecondsOfWeek(_abs64(ballast_index.i64StartTime), &dBallastTime);
			writer.AddIndex(index.id, (DecisionIndexWriter::INDEX_TYPE)index.index_type, index.dNodeTime, (DecisionIndexWriter::INDEX_TYPE)ballast_index.index_type
				, dBallastTime, ballast_index.iStartFrame, ballast_index.iEndFrame);
		}
	}

	if(writer.IsOpen())
		writer.Save();

	return 0;
}

