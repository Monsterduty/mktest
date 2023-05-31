#include "utils.hpp"
#include "global.hpp"
#include "testScriptMethods.hpp"
//#include <iostream>
//#include <fstream>
//#include <iterator>
//#include <filesystem>
//#include <sstream>
//#include <vector>
#include "defaultIncludes.hpp"

static std::string formatString( std::string line )
{
	utils::keepStringStructure(" { ", "{", line);
	utils::keepStringStructure(" } ", "}", line);
	utils::keepStringStructure(" [ ", "[", line);
	utils::keepStringStructure(" ] ", "]", line);
	utils::replaceByString("  ", " ", line );
	utils::replaceByString("\t", " ", line );
	return line;
}

void readConfig( std::string file )
{
	if ( !std::filesystem::exists(file) )
		return;
	std::ifstream streamData( file );
	std::string text = streamData.is_open() ? std::string( std::istreambuf_iterator<char>(streamData),{} ) : "";
	streamData.close();
	if ( text.empty() )
		return;



	std::vector<fileStructPrototype> fileLines = {};
	std::string buffer = "";
	std::stringstream streamText( text );
	//type enclosement works as { string, etc... ].
	bool typeEclosement[] = { false, false };

	auto checkEnclosement = [&](){ for ( bool flag : typeEclosement ) if ( flag ) return true; return false; };

	//parse text.
	while( std::getline( streamText, buffer ) )
	{
		fileStructPrototype aux;
		aux.lineNoFormat = buffer;
		aux.line = fileLines.size() + 1;
		aux.formatedLine = formatString(buffer);

		std::string word = "";
		//saving line words as elements.
		for( int i = 0; i < aux.formatedLine.length(); i++ )
			if ( aux.formatedLine[i] != ' ' && i < aux.formatedLine.length() -1 )
			{
				if ( aux.formatedLine[i] == '"' )
					typeEclosement[0] = !typeEclosement[0];
				word.push_back(aux.formatedLine[i]);
			}
			else
			if ( aux.formatedLine[i] == ' ' && checkEnclosement() && i < aux.formatedLine.length() -1 )
			{
				word.push_back(aux.formatedLine[i]);
			}
			else
			{
				if ( i == aux.formatedLine.length()-1 && aux.formatedLine[i] != ' ' )
					word.push_back( aux.formatedLine[i] );
				if ( !word.empty() )
				{
					aux.lineElements.push_back(word);
					if ( checkEnclosement() )
						for ( int c = 0; c < 2; c++ )
							typeEclosement[c] = false;
					word = "";
				}
			}

		fileLines.push_back(aux);
	}

	readConfigLanguage(fileLines);		
}

bool setEnvironment( std::string name )
{
	int pos = -1;

	for ( int i = 0; i < declaredEnvironments.size(); i++ )
		if ( declaredEnvironments.at(i).name == name )
			pos = i;

	if ( pos < 0 )
	{
		std::cout << "ERROR: the environment [" + name +"] is not declared in the config file" << std::endl;
		return false;
	}

#if defined(__WIN32)
	_putenv( std::string(std::string("LD_LIBRARY_PATH=") + declaredEnvironments.at(pos).libs).c_str() );
	_putenv(  std::string(std::string("LIBRARY_PATH=") + declaredEnvironments.at(pos).libs).c_str() );
	_putenv(  std::string(std::string("CPATH=") + declaredEnvironments.at(pos).includes).c_str() );
	if ( !declaredEnvironments.at(pos).paths.empty() )
		_putenv(  std::string(std::string("PATH=") + declaredEnvironments.at(pos).paths).c_str() );
#else
	setenv( "LD_LIBRARY_PATH", declaredEnvironments.at(pos).libs.c_str(), 1 );
	setenv( "LIBRARY_PATH", declaredEnvironments.at(pos).libs.c_str(), 1 );
	setenv( "CPATH", declaredEnvironments.at(pos).includes.c_str(), 1 );
#endif

	return true;
}