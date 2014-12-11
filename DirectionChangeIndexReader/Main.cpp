// DirectionChangeIndexReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DirectionChangeIndexReader.h"

static void OnError(const char *szBuff)
{
	printf_s("%s", szBuff);
}


int main()
{
	const char *szFilePath = "C:\\develop\\DirectionChangeIndexReader\\20140709-093601-Cam1.direction.xml";

	DirectionChangeIndexReader loader;
	
	if(!loader.Load(szFilePath)){
		OnError(loader.GetLastError());
		return 1;
	}

	for( int i = 0; i < loader.GetNumIndexes();i++)
	{
		DirectionChangeIndexReader::IndexData const& index = loader.GetIndex(i);
	}

	return 0;
}
