#include "utils.hpp"
#include "global.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <filesystem>
#include <sstream>
#include <vector>
#include "testScriptMethods.hpp"

struct syntaxDefinition
{
	std::string name = "";
	std::vector<std::string> fields = {};
	void (*processInfo)( bool*, std::string*, std::vector<std::string>*, errorMessageStruct* ) = nullptr;
};

std::vector<syntaxDefinition> syntax = {
	syntaxDefinition{ "environment", { "libs", "includes" }, environmentObject }
};

void removeChars( std::string &line, const char remove )
{
	while( line.find(remove) != std::string::npos )
	{
		size_t pos = line.find(remove);
		std::string aux = line.substr( 0, pos );
		line = aux + line.substr( pos + 1, line.length() );
	}
}

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
			//if ( ( !checkEnclosement() && i >= aux.formatedLine.length() -1 ) || ( !checkEnclosement() && aux.formatedLine[i] == ' ' ) )
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
		//std::cout << "UNFORMATED: " << aux.lineNoFormat << std::endl;
		//std::cout << "FORMATED  : " << aux.formatedLine << std::endl;
		//std::cout << "ELEMETS   :" << std::endl;
		//for ( auto item : aux.lineElements )
		//	std::cout << "	" << item << std::endl;
		//std::cout << std::endl;
	}

	readConfigLanguage(fileLines);

	//format text.
	//while ( std::getline( streamText, buffer ) )
		
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

	setenv( "LD_LIBRARY_PATH", declaredEnvironments.at(pos).libs.c_str(), 1 );
	setenv( "LIBRARY_PATH", declaredEnvironments.at(pos).libs.c_str(), 1 );
	setenv( "CPATH", declaredEnvironments.at(pos).includes.c_str(), 1 );

	return true;
}

/*
//void readConfigOld( std::string file )
//{
//	if ( !std::filesystem::exists(file) )
//		return;
//	std::ifstream configFile(file);
//	std::istringstream data( std::string( std::istreambuf_iterator<char>(configFile), {} ) );

//	configFile.close();

//	int lineNumber = 1;
//	int currentPos = 0;
//	int matches = 0;
//	bool procedure[3] = { false, false, false };
//	bool readingDefinition = false;

//	std::string buffer = "";

//	errorMessageStruct errorMessage;
//	errorMessage.currentPos = &currentPos;
//	errorMessage.currentLine = &lineNumber;
//	std::string currentWord = "";
//	std::vector<std::string> typoMem = {};
//	std::vector<std::string> typoName = {};
//	char lastChar = 0;
//	while ( std::getline( data, buffer ) )
//	{
//		errorMessage.line = buffer;
//		bool syntaxError = false;
//		int charCounter = 0;
		
//		for ( char c : buffer )
//		{
//			if ( c == '[' )
//			{
//				readingDefinition = true;
//				c = ' ';
			}
			if ( c == ']' )
			{
				readingDefinition = false;
			}

			if ( c == '\t' )
			{
				currentPos++;
				lastChar = c;
				continue;
			}

			//procedure.
			//read word by word.
			if ( c != ' ' && c != '[' && c != ']' )
			{
				currentWord.push_back(c);
			}
			else
			{
				if ( currentWord != "" )
				{
					//reading declaration of a typo like "environment".
					if ( !procedure[0] )
					{
						int pos = 0;
						int biggest = 0;
						for ( auto item : syntax )
						{	
							pos = utils::compString( currentWord, item.name );

							//not match.
							if ( pos > -1 )
							{
								if ( pos > biggest )
								{
									errorMessage.suggestion = "Did you wanted to say: [" + item.name + "]";
									biggest = pos;
								}
							}

							//match
							if ( pos == -1 )
							{
								if ( errorMessage.suggestion.size() > 0 )
									errorMessage.suggestion = "";
								typoMem.push_back(currentWord);
								currentWord = "";
								procedure[0] = true;
							}
						}
						if ( pos > -1 )
						{
							currentPos -= currentWord.size() - biggest;
							errorMessage.errorExtension = currentWord.size() - biggest;
							errorMessage.errorMessage = "ERROR: unrecognized typo: " + currentWord;
						}
						currentWord = "";
					}
					else
					if ( procedure[0] && !procedure[1] )
					{
						typoName.push_back(currentWord);
						currentWord = "";
						procedure[1] = true;
					}
					else
					if ( procedure[0] && procedure[1] && !procedure[2] )
					{
						for ( auto item : syntax )
						if ( typoMem.back() == item.name )
							if ( item.processInfo != nullptr )
							item.processInfo( &procedure[2], &currentWord, &item.fields, &errorMessage );
					}
				}
				else 
				{
					if ( readingDefinition && ( typoMem.size() > typoName.size() ) )
					{
						errorMessage.errorMessage = "ERROR: expected name for identify the typo";
						errorMessage.suggestion = "The correct syntax is [ <Typo> <Name> <Value> ] Example: " + typoMem.back() + " name [ someVal = {} ]";
						currentPos--;
						errorMessage.errorExtension = 1;
					}	
				}
			}
			//check typo, name and value.
			//save is elsewhere.

			if ( errorMessage.errorMessage != "" )
			{
				std::string location = file + ":" + std::to_string(lineNumber) + ":";
				if ( errorMessage.errorMessage != "" )
					std::cout << errorMessage.errorMessage << std::endl;
				std::cout << location << errorMessage.line << std::endl;
				std::cout << std::string(location.size() + *errorMessage.currentPos, ' ' ) + std::string( errorMessage.errorExtension, '^' ) << std::endl;
				if ( errorMessage.suggestion.size() > 0 )
					std::cout << errorMessage.suggestion << std::endl;
				return;
			}

			currentPos++;
			lastChar = c;
		}

		if ( syntaxError )
			break;

		lineNumber++;
		currentPos = 0;
	}

	std::cout << "ABOUT TO PRINT OUTPUT" << std::endl;
	for ( auto item : typoMem )
		std::cout << "TYPO: " << item << std::endl;
	for ( auto item : typoName )
		std::cout << "NAME: " << item << std::endl;
}
*/
