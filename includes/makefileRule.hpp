#ifndef MAKEFILE_RULE
#define MAKEFILE_RULE
//#include <string>
//#include <iostream>
#include "defaultIncludes.hpp"

class makefileRule
{
	std::string goalFile = "";
	std::string sourceFiles = "";
	std::string instructions = "";

public:
	makefileRule( std::string goal, std::string source, std::string procedure, std::string compiler );

	std::string provideRule();

	std::string provideGoal();

};

void generateMakeFile( std::string flags );
#endif
