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
	std::string dependencies = "";
	std::string compileFlags = "";

public:
	makefileRule( std::string goal, std::string source, std::string deps );

	std::string provideRule();

	std::string provideGoal();

	void setCompileFlags( std::string flags );
};

void generateMakeFile( std::string flags );
#endif
