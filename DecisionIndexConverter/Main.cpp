// DecisionIndexConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DecisionIndexWriter.h"
#include "DecisionIndexReader.h"
#include <string>
#include <fstream>

bool FileExists(const char *szFilePath)
{
	std::ifstream in(szFilePath);
	return !(!in);
}

std::string GetFileExtension(const std::string& FileName)
{
	std::string ssExtension = "";
	std::string ssTmp = FileName;
	size_t found = ssTmp.find_last_of(".");
	while (true)
	{
		found = ssTmp.find_last_of(".");
		if(found == std::string::npos)
			break;
		ssExtension = "." + ssTmp.substr(found + 1) + ssExtension;
		ssTmp = ssTmp.substr(0, found);
	}
	if(ssExtension.length())
		ssExtension = ssExtension.substr(1,ssExtension.length()-1);
	return ssExtension;
}

std::string GetFileName(const char * szFilePath)
{
	std::string ssFilePath = szFilePath;
	size_t found = ssFilePath.find_last_of("\\");
	if (found == std::string::npos)
	{
		return "";
	}
	std::string ssFile = ssFilePath.substr(found + 1);
	return ssFile;
}

//Gets the directory location from a file path and known file extension
std::string GetFileDirectory(const char * szFilePath)
{
	//find last "\\".
	std::string str = szFilePath;
	size_t found = str.find_last_of("\\");
	return str.substr(0, found + 1);
}

std::string GetFileNameNoExtension(const char *szFilePath)
{
	std::string ssFilename = GetFileName(szFilePath);
	std::string ssFileExtension = GetFileExtension(szFilePath);
	return ssFilename.substr(0, ssFilename.length() - ssFileExtension.length() - 1);
}

void help()
{
	printf("DirectionIndexConverter.exe <filepath>");
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		help();
		return 1;
	}

	const char *szFilePath_in = argv[1];
	//const char *szFilePath_in = "X:\\Projects\\801 - Map Matching\\Exe\\BallastScanner\\Data\\export\\tiny4_121943.decision.xml";
	std::string ssDir = GetFileDirectory(szFilePath_in);
	std::string ssFileName_out = GetFileNameNoExtension(szFilePath_in);
	std::string ssFilePath_out = ssDir + ssFileName_out + "_reversed";

	if(!FileExists(szFilePath_in))
	{
		printf_s("\n");
		printf_s("File does not exist: %s", szFilePath_in);
		printf_s("\n");
		system("PAUSE");
		return 1;
	}

	DecisionIndex::Writer writer;
	DecisionIndex::Reader reader;

	if(!reader.Load(szFilePath_in))
	{
		return 1;
	}

	if(!writer.Open(ssFilePath_out.c_str()))
	{
		return 1;
	}

	DecisionIndex::Data data;
	while(reader.GetDecision(data))
	{
		//flip direction
		if(data.direction == DecisionIndex::TurnDirection::Direction_Left)
			data.direction = DecisionIndex::Direction_Right;
		else if(data.direction == DecisionIndex::TurnDirection::Direction_Right)
			data.direction = DecisionIndex::Direction_Left;
		else
			data.direction = DecisionIndex::Direction_Unknown;

		writer.AddIndex(data);
	}

	writer.Save();
	writer.Close();

	return 0;
}
