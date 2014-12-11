#pragma once


/*
Contains the data structure used by DecisionIndexReader and DecisionIndexWriter.
***************************************************************************************************
Authors :
Michael McGlynn (MRM)
***************************************************************************************************
Ver: By:  Date:       Comment:
1.0  MRM  11/12/2014  First version
***************************************************************************************************
*/

namespace DecisionIndex
{

	enum TurnDirection{ Direction_Left, Direction_Right, Direction_Unknown };
	struct DecisionIndexData
	{
		DecisionIndexData()
		{
			iId = 0;
			direction = TurnDirection::Direction_Unknown;
			i64Time = 0;
			dConfidence = 1.0;
		}

		DecisionIndexData(const DecisionIndexData & data)
		{
			*this = data;
		}

		DecisionIndexData(int _iId, TurnDirection _Direction, __int64 _i64Time, double _dConfidence)
			: iId(_iId), direction(_Direction), i64Time(_i64Time), dConfidence(_dConfidence)
		{
		}

		int iId;
		TurnDirection direction;
		__int64 i64Time;
		double dConfidence;
	};

};