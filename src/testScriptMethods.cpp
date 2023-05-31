#include "global.hpp"
#include "testScriptMethods.hpp"
#include "programEnvironmentStruct.hpp"
#include "utils.hpp"
//#include <iostream>
//#include <vector>
#include "defaultIncludes.hpp"

programEnvironmentStruct temporalStruct;
std::vector<std::string> lineElements = {};

struct filePos
{
	int line;
	std::string lineText;
	int linePos;
};

static void replaceIf( std::string *statements, std::string replacement, std::string &source )
{
	int i = 0;
	while ( statements[i] != "" )
	{
		while ( source.find( statements[i] ) != std::string::npos )
		{
			size_t pos = source.find(statements[i]);

			if ( pos == std::string::npos )
				continue;
			
			std::string aux = source.substr(0, pos);
			source = aux + source.substr(pos+statements[i].size(), source.length() );
		}
		i++;
	}


}

std::vector<filePos> positionInFile = {};
void environmentObject( bool *stopCalling, std::string *currentText, std::vector<std::string> *fields, errorMessageStruct *errorMessage )
{
	if ( currentText->find('}') != std::string::npos && currentText->size() > 1)
	{
		utils::keepStringStructure(" } ", "}", *currentText );
		utils::replaceByString("  ", " ", *currentText);
		std::string aux = "";

		auto func = [&]( std::string word ){
			lineElements.push_back(aux);
				aux.clear();
				int posInLine = *errorMessage->currentPos - currentText->size() + 1;
				positionInFile.push_back( filePos{ *errorMessage->currentLine, errorMessage->line, posInLine } );
		};

		for ( char c : *currentText )
			if ( c != ' ' )
				aux.push_back(c);
			else 
			{
				if ( aux.size() )
					func(aux);
			}
		if ( !aux.empty() )
			func(aux);
	}
	else
	if ( currentText->find('{') != std::string::npos && currentText->at(0) != '{' )
	{
		//adding field name.
		size_t pos = currentText->find('{');
		lineElements.push_back( currentText->substr(0, pos) );
		int posInLine = *errorMessage->currentPos - currentText->size() + 1;
		positionInFile.push_back( filePos{ *errorMessage->currentLine, errorMessage->line, posInLine } );

		//adding curly brace.
		lineElements.push_back( std::string( 1, currentText->at(pos) ) );
		posInLine = *errorMessage->currentPos - currentText->size() + pos + 1 ;
		positionInFile.push_back( filePos{ *errorMessage->currentLine, errorMessage->line, posInLine } );
	}
	else 
	{
		lineElements.push_back( *currentText );
		int posInLine = *errorMessage->currentPos - currentText->size() + 1;
		positionInFile.push_back( filePos{ *errorMessage->currentLine, errorMessage->line, posInLine } );
	}
	*currentText = "";

	if ( lineElements.back() == "}" )
	{
		bool check[] = { false, false, false, false };
		int biggest = 0;
		int pos = 0;
		std::string codeSuggestion = "";
		//checking field name, in this case are available libs and includes.
		for ( auto item : *fields )
		{
			pos = utils::compString(lineElements.at(0), item);

			if ( pos == -1 )
			{
				check[0] = true;
			}
			else
			{
				if ( pos > biggest )
					codeSuggestion = item;
			}
		}

		if ( lineElements.at(1) == "=" )
			check[1] = true;

		if ( lineElements.at(2) == "{" )
			check[2] = true;

		for ( auto item : lineElements )
		std::cout << "item: " << item << std::endl;
		for ( int i = 3; i <= lineElements.size() -1; i++ )
			if ( lineElements.at(i).at(0) == '"' && lineElements.at(i).back() == '"' )
				check[3] = true;
			else
			{
			 	check[3] = false;
				break;
			}

		if ( lineElements.size() )
			lineElements.clear();
		if ( positionInFile.size() )
			positionInFile.clear();

		if ( !check[0] )
		{
			errorMessage->errorMessage = "ERROR: the environment object doesn't contains a field called: [" + lineElements.at(0) + "]";
			errorMessage->suggestion = "Did you wanted to say [" + codeSuggestion + "]?";
			errorMessage->errorExtension = 1;
			*errorMessage->currentLine = positionInFile.at(0).line;
			errorMessage->line = positionInFile.at(0).lineText;
			*errorMessage->currentPos = positionInFile.at(0).linePos;
			return;
		}

		if ( !check[1] )
		{
			errorMessage->errorMessage = "ERROR: to assing a value to an object field you need to provide a '=' character.";
			errorMessage->suggestion = "Example: " + lineElements.at(0) + " = { value }";
			errorMessage->errorExtension = (positionInFile.at(1).linePos ) - ( (lineElements.at(0).size() - 2) + positionInFile.at(0).linePos );
			*errorMessage->currentLine = positionInFile.at(1).line;
			errorMessage->line = positionInFile.at(1).lineText;
			*errorMessage->currentPos = positionInFile.at(0).linePos + lineElements.at(0).size() -1;
			return;
		}

		if ( !check[2] )
		{
			errorMessage->errorMessage = "ERROR: missing curly brace at the start of the assignation.";
			*errorMessage->currentLine = positionInFile.at(2).line;
			errorMessage->line = positionInFile.at(2).lineText;
			errorMessage->errorExtension = 1;
			*errorMessage->currentPos = positionInFile.at(2).linePos;
			errorMessage->suggestion = "Example: " + lineElements.at(0) + " = { \"value\" }";
			return;
		}

		if ( !check[3] )
		{
			errorMessage->errorMessage = "ERROR: content requires to be in string format";
			return;
		}
	}
}

class baseMethod
{
	public:
	virtual void receiveInformation( std::string, errorMessageStruct** ) = 0;
	virtual bool endMethod() = 0;
};

//std::string keyWords[] = { "environment" };
struct keywordsRequirements
{
	std::string keyWord = "";
	std::vector<std::string> requiredElements = {};
	baseMethod *method = nullptr;
};

//===========================>
// TESTSCRIPT METHODS
//===========================>
class environmentMethod : public baseMethod
{
	bool end = false;

	enum FIELDS
	{
		LIBS,
		INCLUDES,
		PATHS,
		ARGS
	};

	int configToDefine = -1;

	std::vector<std::string> allowedSyntax = { "[","NAME","=", "STRING", "]" };
	std::vector<std::string> fields = { "libs", "includes", "paths", "args" };
	int step = 0;
	std::vector<std::string> savedElements = {};
	public:
	void receiveInformation( std::string element, errorMessageStruct **errorMessage ) override
	{
		bool error = true;
		int errorNumber = 1;

		if ( element == allowedSyntax.at(0) )
		{
			end = false;
			error = false;
		}

		if ( element == allowedSyntax.back() )
		{
			end = true;
			step = 0;
			int configToDefine = -1;
			return;
		}

		for ( auto item : allowedSyntax )
			if ( element == item )
				error = false;

		if ( allowedSyntax.at(step) == "]" &&  element != "]" )
		{
			step = 1;
			configToDefine = -1;
		}

		if ( end )
			return;

		if ( allowedSyntax.at(step) == "NAME" )
			for ( int i = 0; i < fields.size(); i++ )
				if ( fields.at(i) == element )
				{
					error = false;

					configToDefine = i;
					step++;
					return;
				}

		if ( allowedSyntax.at(step) == "STRING" )
		{
			error = false;
			if ( element.empty() )
				error = true;
			if ( element[0] == '"' )
				element = element.substr(1, element.size());
			if ( element.back() == '"' )
				element = element.substr(0, element.size() - 1);

			if ( !error )
			switch (configToDefine) {
				case LIBS:
					declaredEnvironments.back().libs = element;
					break;
				case INCLUDES:
					declaredEnvironments.back().includes = element;
					break;
				case PATHS:
					declaredEnvironments.back().paths = element;
					break;
				case ARGS:
					declaredEnvironments.back().args = element;
					break;
				default:
					error = true;
					break;
			}
		}

		if ( error )
		{
			switch (errorNumber) {
				case 1: 
				{
					auto suggestion = [&]()
					{ 
						std::string ret = "";
						if ( allowedSyntax.at(step) == "NAME" )
						{
							ret = "Allowed field names: ";
							for( auto item : fields )
								ret += item + ", ";
							ret = ret.substr(0, ret.size()-2);
						}
						return ret;
					};
					*errorMessage = new errorMessageStruct{
						.errorMessage = "Syntax error: wrote [" + element + "] rather than [" + allowedSyntax.at(step) + "]",
						.suggestion = suggestion(),
					};
					break;
				}
				default:
					break;
			}
		}
		step++;
	}
	bool endMethod() override
	{
		return end;
	}
};

class defaultMethod : public baseMethod
{
	bool end = false;
	//bool reading = false;
	std::vector<std::string> names = { "editor", "compiler", "c++", "template", "args" };
	enum configNames
	{
		EDITOR,
		COMPILER,
		CPP,
		TEMPLATES,
		ARGS
	};
	std::vector<std::string> allowedSyntax = { "[","NAME","=", "STRING", "]" };
	//std::vector<std::string> savedElements = {};
	int step = 0;
	int configToDefine = -1;
public:
	void receiveInformation( std::string element, errorMessageStruct **errorMessage ) override
	{
		bool error = true;
		int errorNumber = 1;

		if ( element == allowedSyntax.at(0) )
		{
			end = false;
			error = false;
		}

		if ( element == allowedSyntax.back() )
		{
			end = true;
			return;
		}

		for ( auto item : allowedSyntax )
			if ( element == item )
				error = false;

		if ( allowedSyntax.at(step) == "]" &&  element != "]" )
		{
			step = 1;
			configToDefine = -1;
		}

		if ( end )
			return;

		if ( allowedSyntax.at(step) == "NAME" )
			for ( int i = 0; i < names.size(); i++ )
				if ( names.at(i) == element )
				{
					error = false;

					configToDefine = i;
					step++;
					return;
				}

		if ( allowedSyntax.at(step) == "STRING" )
		{
			error = false;
			switch (configToDefine) {
			case EDITOR:
				editor = element + " ";
				break;
			case COMPILER:
				compiler = element;
				break;
			case ARGS:
				if ( element.empty() )
				{
					error = true;
					break;
				}
				if ( element[0] == '"' )
					element = element.substr(1, element.size());
				if ( element.back() == '"' )
					element = element.substr(0, element.size() - 1);
				programsArgs = element;
				break;
			default:
				error = true;
				break;
			}
		}

		if ( error )
		{
			switch (errorNumber) {
				case 1: 
				{
					*errorMessage = new errorMessageStruct{
						.errorMessage = "Syntax error: wrote [" + element + "] rather than [" + allowedSyntax.at(step) + "]",
					};
					break;
				}
				default:
					break;
			}
		}
		step++;
	}
	bool endMethod() override
	{
		if ( end )
		{
			step = 0;
			configToDefine = -1;
			//savedElements.clear();
		}
		return end;
	}
};
//===========================>

std::vector<keywordsRequirements> keyWords = {};

static void defineKeyWords( std::vector<keywordsRequirements> &objective )
{
	keywordsRequirements data;
	data.keyWord = "environment";
	data.requiredElements = { "{", "}" };
	data.method = new environmentMethod();

	objective.push_back(data);

	data.keyWord = "default";
	data.method = new defaultMethod();

	objective.push_back(data);
}

void readConfigLanguage( std::vector<fileStructPrototype> fileData )
{
	errorMessageStruct *errorMessage = nullptr;
	defineKeyWords(keyWords);

	bool codeSegmentsVerification[] = { false, false, false };
	int sectionReaded[] = { -1, -1, -1};
	int keyWordToUse = -1;

	//for each line.
	for ( auto data : fileData )
	{
		if ( !errorMessage )
			//for each line element.
			for ( int i = 0; i < data.lineElements.size(); i++ )
			{
				//once completed, reseting all the reading proccess.
				if ( codeSegmentsVerification[0] && codeSegmentsVerification[1] && codeSegmentsVerification[2] )
				{
					for ( int c = 0; c < 3; c++ )
						codeSegmentsVerification[c] = false;
					keyWordToUse = -1;
					sectionReaded[0] = -1;
				}

				//the code structure must start with a type declaration every time.
				if ( !codeSegmentsVerification[0] )
				{
					for ( int f = 0; f < keyWords.size(); f++ )
						if ( keyWords.at(f).keyWord == data.lineElements.at( sectionReaded[0] + 1 ) )
						{
							codeSegmentsVerification[0] = true;
							keyWordToUse = f;
							//std::cout << "KEYWORD: " << keyWords.at(f).keyWord << " NAME: " << data.lineElements.at( sectionReaded[0] + 2 ) << std::endl;
							break;
						}
					sectionReaded[0] = i;
					continue;
				}
				if ( !codeSegmentsVerification[0] )
					break;

				if ( codeSegmentsVerification[0] && !codeSegmentsVerification[1] )
				{
					if ( keyWords.at( keyWordToUse ).keyWord == "environment" )
					{
						environmentToSetClass environment;
						environment.name = data.lineElements.at(i);
						declaredEnvironments.push_back(environment);
					}

					codeSegmentsVerification[1] = true;
					continue;
				}
				if ( !codeSegmentsVerification[1] )
					break;

				if ( codeSegmentsVerification[0] && codeSegmentsVerification[1] && !codeSegmentsVerification[2] )
				{
					if ( keyWords.at(keyWordToUse).method != nullptr )
						keyWords.at(keyWordToUse).method->receiveInformation( data.lineElements.at(i), &errorMessage );
					if ( keyWords.at(keyWordToUse).method->endMethod() )
					{
						codeSegmentsVerification[2] = true;
						continue;
					}
					if ( errorMessage )
					{
						errorMessage->currentLine = new int(data.line);
						errorMessage->line = data.lineNoFormat;
						break;
					}
				}

			}

		if ( !codeSegmentsVerification[0] && !errorMessage )
		{
			std::cout << "creating error message" << std::endl;
			errorMessage = new errorMessageStruct;
			errorMessage->errorMessage = "undefined keyWord";
		}
	
		if ( errorMessage )
		{
			std::cout << "config language ERROR:" << std::endl;
			if ( !errorMessage->errorMessage.empty() )
				std::cout << errorMessage->errorMessage << std::endl;
			if ( errorMessage->currentLine )
			{
				std::cout << *errorMessage->currentLine << ':';
				if ( !errorMessage->line.empty() )
					std::cout << errorMessage->line;
				std::cout << std::endl;
			}
			if ( !errorMessage->suggestion.empty() )
				std::cout << errorMessage->suggestion << std::endl;
			delete errorMessage;
			exit(1);
		}
	}

}
