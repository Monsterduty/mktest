#include "makefileRule.hpp"
#include "global.hpp"

makefileRule::makefileRule( std::string goal, std::string source, std::string procedure, std::string compiler )
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
	instructions = "	" + compiler + " " + sourceFiles.substr(0, (sourceFiles.find(' ') != std::string::npos ? sourceFiles.find(' ') : sourceFiles.length() ) ) + procedure;
	sourceFiles += "\n\n";
}

std::string makefileRule::provideGoal()
{
	std::string ret = "";
	for ( int i = 0; i < goalFile.length(); i++ )
	 if ( goalFile[i] != '\n' && goalFile[i] != ':' )
	 	ret.push_back( goalFile[i] );
	return ret;
}

std::string makefileRule::provideRule()
{
	return goalFile + sourceFiles + instructions;
}

//creating required Makefile with all the required arguments, flags and files for build
void generateMakeFile( std::string flags )
{
	using namespace std;
	ofstream mkfile( path + "/Makefile" );

	string depsGoals = " ";
	for ( auto goal : mkfileRules )
		depsGoals += goal.provideGoal() + " ";

	string objFiles = "";
	for ( auto object : mkfileRules )
		if ( object.provideGoal().find(".o") != string::npos )
			objFiles += " " + object.provideGoal();

	//executable is the goal, ( file + depsGoals ) are all goal need to take in account
	//and compiler with the objFiles and flags are the procedure,
	//file g++ result in: g++ (execuable = text.cpp) (objFile = animFIle.o anotherFile.o)
	makefileRule mainRule( executable, file + depsGoals, objFiles + " " + flags, compiler );

	mkfile << mainRule.provideRule();

	if ( mkfileRules.size() > 0 )
		for ( auto rule : mkfileRules )
			mkfile << rule.provideRule();

	mkfile.close();
};
