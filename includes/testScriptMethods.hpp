#pragma once
//#include <string>
//#include <vector>
#include "defaultIncludes.hpp"

	//struct to determine line positions.
struct fileStructPrototype
{
	int line = 0;
	int linePos = 0;
	std::string lineNoFormat = "";
	std::string formatedLine = "";
	std::vector<std::string> lineElements = {};
};

void readConfigLanguage( std::vector<fileStructPrototype> fileData );

struct errorMessageStruct;
void environmentObject( bool *stopCalling, std::string *currentText, std::vector<std::string> *fields, errorMessageStruct *errorMessage );

class errorMessageStruct
{
public:
	std::string errorMessage = "";
	std::string suggestion = "";
	std::string line = "";
	int errorExtension = 0;
	int *currentPos = nullptr;
	int *currentLine = nullptr;
	~errorMessageStruct()
	{
		if ( currentPos != nullptr )
			delete currentPos;
		if ( currentLine != nullptr )
			delete currentLine;
	}
};

class environmentToSetClass
{
	public:
		std::string name = "";
		std::string libs = "";
		std::string includes = "";
		std::string paths = "";
		std::string args = "";
};

inline std::vector<environmentToSetClass> declaredEnvironments = {};
