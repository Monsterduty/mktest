#include <string>
#include <iostream>

class makefileRule
{
	std::string goalFile = "";
	std::string sourceFiles = "";
	std::string instructions = "";

public:
	makefileRule( std::string goal, std::string source, std::string procedure )
	{
		//clean the name of the goal file
		goal = goal.find('/') != std::string::npos ? goal.substr(1, goal.length() ) : goal;

		//clean first source name
		source = source.find('/') < 2 ? source.substr(1, source.length() ) : source;

		//if there's another files, clean teir names
		for( auto c : source )
			if ( c != '/' || sourceFiles.back() != ' ' )
				sourceFiles.push_back(c);

		//create the structure of the rule
		goalFile = "\n\n" + goal + ": ";
		instructions = "	" + procedure + " " + sourceFiles.substr(0, (sourceFiles.find(' ') != std::string::npos ? sourceFiles.find(' ') : sourceFiles.length() ) );
		sourceFiles += "\n\n";
	}

	std::string provideRule()
	{
		return goalFile + sourceFiles + instructions;
	}

	std::string provideGoal()
	{
		std::string ret = "";
		for ( int i = 0; i < goalFile.length(); i++ )
		 if ( goalFile[i] != '\n' && goalFile[i] != ':' )
		 	ret.push_back( goalFile[i] );
		return ret;
	}
};