#include "makefileRule.hpp"
#include "global.hpp"
#include "utils.hpp"

static std::string normalizeNames( std::string names )
{
	std::string ret = "";
	std::vector<std::string> vec = {};
		
	//separate words into strings.
	for ( char &c : names )
		if ( c != ' ' && &c != &names.back() )
			ret.push_back(c);
		else
		{
			if ( ret.empty() )
				continue;
			if ( ret[0] == '/' )
				ret = ret.substr( 1, ret.length() );
			if ( c != ' ' && &c == &names.back() )
				ret.push_back(c);
			vec.push_back(ret);
			ret.clear();
		}

	//erase repetitions.
	for ( size_t i = 0; i < vec.size(); i++ )
		while( std::count( vec.begin(), vec.end(), vec.at(i) ) > 1 )
			vec.erase( vec.begin() + i );

	//create a sigle string with the normalized words.
	for ( std::string &item : vec )
		ret += item + " ";

	return ret;
}

/* 
 * Maybe future implementation at utils.hpp
 *
static std::string getWordsWith( std::string pattern, std::string line )
{
	std::string ret = "", aux = "";
	std::vector<std::string> vec = {};

	for ( char &c : line )
		if ( c != ' ' && &c != &line.back() )
			ret.push_back(c);
		else
		{
			if ( ret.empty() )
				continue;
			if ( c != ' ' && &c == &line.back() )
				ret.push_back(c);
			if ( ret.find(pattern) != std::string::npos )
				aux += ret + " ";
			ret.clear();
		}

	if ( aux.size() > 1 && aux.back() == ' ' )
		aux = aux.substr( 0, aux.size() -1 );
	return aux;
}
*/

makefileRule::makefileRule( std::string goal, std::string source, std::string deps )
{
	//clean the name of the goal file
	goal = goal.find('/') != std::string::npos ? goal.substr(1, goal.length() ) : goal;

	std::string nameBuffer = "";
	
	//separate and clean of '/' sources names.
	sourceFiles = normalizeNames(source);

	//create the structure of the rule
	goalFile = goal;
	dependencies = normalizeNames( sourceFiles + " " + deps );
	if ( dependencies.find( goalFile ) != std::string::npos )
		utils::replaceByString( goalFile, "", dependencies);
	//sourceFiles += getWordsWith( ".o", dependencies);
	sourceFiles = normalizeNames(sourceFiles);

	std::string auxFlag = " ";
	if ( goalFile.find( ".o" ) == goalFile.length() - 2 )
		auxFlag += " -c ";

	instructions = "\t" + compiler + auxFlag + sourceFiles + "-o " + goalFile;
}

std::string makefileRule::provideGoal()
{
	return goalFile;
}

std::string makefileRule::provideRule()
{
	return goalFile + ": " + dependencies + "\n\n" + instructions + " " + compileFlags + "\n\n";
}

void makefileRule::setCompileFlags( std::string flags )
{
	compileFlags += flags;
}

//creating required Makefile with all the required arguments, flags and files for build
void generateMakeFile( std::string flags )
{
	using namespace std;
	ofstream mkfile( path + "/Makefile" );

	string depsGoals = " ";
	for ( auto &goal : mkfileRules )
		depsGoals += goal.provideGoal() + " ";

	string objFiles = "";
	for ( auto &object : mkfileRules )
		if ( object.provideGoal().find(".o") != string::npos )
			objFiles += " " + object.provideGoal();

	//executable is the goal, ( file + depsGoals ) are all goal need to take in account
	//and compiler with the objFiles and flags are the procedure,
	//file g++ result in: g++ (execuable = text.cpp) (objFile = animFIle.o anotherFile.o)
	makefileRule mainRule( executable, objFiles, depsGoals );
	mainRule.setCompileFlags( customCompileArgs ? customCompileArgsString : flags );

	mkfile << mainRule.provideRule();

	if ( mkfileRules.size() > 0 )
		for ( auto &rule : mkfileRules )
			mkfile << rule.provideRule();

	mkfile.close();
};
