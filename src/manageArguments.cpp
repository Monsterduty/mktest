#include "manageArguments.hpp"
#include "global.hpp"
#include "mktestInfoOutput.hpp"
#include "configParser.hpp"
#include "fileOperations.hpp"
#include "cacheOperations.hpp"
#include "terminalFontStyles.hpp"
#include "testScriptMethods.hpp"

//argument conflict handler
bool checkConflicts( std::string arg )
{
	std::string data[] = { "--new", "--help", "--args", "--editor", "--saveProj", "--template", "clearCache", "--keep",  "--setEnv", "--verbose", "--config", "--compileCommands", "-v", "--path", "--daemon" };

	for ( auto args : data )
		if ( arg == args )
			return true;
	return false;
}

namespace terminal_methods
{
	static void printVersion( std::string data )
	{
		std::cout << version << " - " << buildType << std::endl;
		exit(0);
	}
	static void printHelp( std::string data )
	{
		help();
		exit(0);
	}

	static void setProgramArgs( std::string data )
	{
			programsArgs += data;
	}

	static void createNewFileOrProject( std::string optionalFile )
	{
		if ( !std::filesystem::exists(path) )
			std::filesystem::create_directory(path);
		if ( !std::filesystem::exists(path) )
		{
			throwMessage("could not create the main working directory at: " + path, THROW_CODE::ERROR );
			std::cout << "where: [--new] argument handler position" << std::endl;
			exit(1);
		}

		if ( optionalFile == "" )
		{
			for ( auto item : std::filesystem::directory_iterator(path) )
				std::filesystem::remove(item);
			createNecesaryFiles("test.cpp");
		}
		else 
			createNecesaryFiles(optionalFile);
	}

	static void saveProject( std::string pathToSave )
	{
		if ( !pathToSave.empty() )
				saveCode( pathToSave );
			else
				saveCode();
		exit(0);
	}

	static void changeWorkingPath( std::string newPath )
	{			
		if ( !std::filesystem::exists( newPath ) )
		{
			throwMessage( "the path [" + newPath + "] doesn't exits!\nWhat should we do? [create/exit]: ", THROW_CODE::WARNING );
			std::string response = "";
			std::cin >> response;
			if ( response.empty() )
				exit(0);

			if ( response.at(0) == 'c' )
			{
				std::filesystem::create_directory( newPath );
				if ( !std::filesystem::exists( newPath ) )
				{
					throwMessage( "couldn't create the new directory on [" + newPath + "]", THROW_CODE::ERROR );
					exit(0);
				}
			}
			else
			{
				std::cout << "\naborting\n";
				exit(0);
			}
		}
		if ( newPath[0] == '/' || newPath.find("://") < 2 )
			path = newPath;
		else
			path = std::filesystem::current_path().string() + "/" + newPath;
	}

	static void keepEditingFile( std::string data )
	{
		if ( !std::filesystem::exists(path) )
		{
			throwMessage( "not working directoty to keep file", THROW_CODE::WARNING );
		}
		modArgs += " keep";
		if ( file == DEFAULT_MAIN_FILE )
			return;
		//if the [ file ] variable has an slash character, is nedded to romove that
		std::string read = file.find('/') != std::string::npos ? file.substr(1, file.length() ) : file;

		//search for the code file
		for ( auto dir : std::filesystem::directory_iterator( path ) )
			if ( dir.path().string().find( read ) != std::string::npos )
			{
				//stream to read the code file.
				std::ifstream ReadcodeFile( path + "/" + read );
				std::string buffer = "";

				//stracting the information from the code file.
				std::istreambuf_iterator<char> begin( ReadcodeFile ), end;
				while ( begin != end )
					buffer += *begin, begin++;

				ReadcodeFile.close();

				//stream to rewrite the file with the mod arg [ keep ].
				std::ofstream writeCodeFile( path + "/" + read );

				//adding mod args and the code to the buffer.
				buffer = modArgs + "\n" + buffer;

				//adding the whole code to the file
				writeCodeFile << buffer;

				writeCodeFile.close();

				//write in the cache file with file keep editing
				writeCache("fileToEdit", read);
			
				break;
			}
	}

	static void changeConfigFile( std::string fileName )
	{
		if ( !std::filesystem::exists( fileName ) )
		{
			std::cout << "ERROR: config file [" + fileName + "] doesn't exist!" << std::endl;
			exit(1);
		}
		configFile = fileName;
		if ( std::filesystem::exists(configFile) )
			readConfig( configFile );
	}

	static void changeEditor( std::string newEditor )
	{
			editor = std::string( newEditor )  + " ";
	}

	static void prepareTemplate( std::string templateName )
	{
		createExampleFile( templateName );
	}

	static void clearCache( std::string data )
	{
		deleteFile( cacheFileName );
	}

	static void enableVerbose( std::string data )
	{
		verbose = true;
	}

	static void enableDaemonMode( std::string data )
	{
		generateMakefileWhileCodding = true;
	}

	static void prepareEnvironment( std::string enviroment )
	{
		if ( !setEnvironment( enviroment ) )
			exit(1);
		
		std::string color = std::string(TEXT_BOLD) + std::string(TEXT_CYAN);
		for ( auto item : declaredEnvironments )
			if ( item.name == enviroment )
			{
				if ( !item.paths.empty() && verbose )
					verboseOutput += color + "PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("PATH") != nullptr ? std::getenv("PATH") : "" ) + "\n";
				if ( !item.args.empty() )
				{
					customCompileArgs = true;
					customCompileArgsString = item.args;
				}
				break;
			}

		if ( !verbose )
			return;

		verboseOutput += color;
		verboseOutput += "enviroment: ";
		verboseOutput += TEXT_RESET;
		verboseOutput += enviroment + "\n";
		verboseOutput += color + "LD_LIBRARY_PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("LD_LIBRARY_PATH") != nullptr ? std::getenv("LD_LIBRARY_PATH") : "" ) + "\n";
		verboseOutput += color + "LIBRARY_PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("LIBRARY_PATH") != nullptr ? std::getenv("LIBRARY_PATH") : "" ) + "\n";
		verboseOutput += color + "CPATH:" + std::string(TEXT_RESET) + std::string( std::getenv("CPATH") != nullptr ? std::getenv("CPATH") : "" ) + "\n";
	}
	
	static void addCompileCommands( std::string commands )
	{
		customCompileArgs = true;
		customCompileArgsString = commands;
	}
}

enum REQUIRE_ARGUMENTS_ENUM{
	YES = 1,
	NO = 2,
	OPTIONAL = 3
};
enum PRIORITY_HIERACHY_ENUM{
	LOW = 1,
	MEDIUM = 2,
	HIGHT = 3,
	EXTREME = 4
};

struct argumentStruct
{
	std::string name = "";
	std::string value = "";
	uint8_t requireArgument = 0;
	uint8_t priority = 0;
	void (*method)(std::string) = nullptr;
};
static const inline std::vector<argumentStruct> argumentsAvailable = 
{
	argumentStruct{ .name = "--new", .requireArgument = OPTIONAL, .method = terminal_methods::createNewFileOrProject },
	argumentStruct{ .name = "--help", .requireArgument = NO, .priority = LOW, .method = terminal_methods::printHelp  },
	argumentStruct{ .name = "--args", .requireArgument = YES,  .priority = LOW, .method = terminal_methods::setProgramArgs  },
	argumentStruct{ .name = "--editor", .requireArgument = YES, .priority = LOW, .method = terminal_methods::changeEditor },
	argumentStruct{ .name = "--saveProj", .requireArgument = OPTIONAL, .priority = LOW, .method = terminal_methods::saveProject },
	argumentStruct{ .name = "--template", .requireArgument = YES, .priority = MEDIUM, .method = terminal_methods::prepareTemplate },
	argumentStruct{ .name = "--clearCache", .requireArgument = NO, .priority = LOW, .method = terminal_methods::clearCache },
	argumentStruct{ .name = "--keep", .requireArgument = NO, .priority = LOW, .method = terminal_methods::keepEditingFile },
//	argumentStruct{ .name = "--standAlone", .requireArgument = NO }, //not implemented yet.
	argumentStruct{ .name = "--setEnv", .requireArgument = YES, .priority = LOW, .method = terminal_methods::prepareEnvironment },
	argumentStruct{ .name = "--verbose", .requireArgument = NO, .priority = HIGHT, .method = terminal_methods::enableVerbose },
	argumentStruct{ .name = "--config", .requireArgument = YES, .priority = EXTREME, .method = terminal_methods::changeConfigFile },
	argumentStruct{ .name = "--compileCommands", .requireArgument = YES, .priority = LOW, .method = terminal_methods::addCompileCommands },
	argumentStruct{ .name = "-v", .requireArgument = NO, .priority = LOW, .method = terminal_methods::printVersion },
	argumentStruct{ .name = "--path", .requireArgument = YES, .priority = LOW, .method = terminal_methods::changeWorkingPath },
	argumentStruct{ .name = "--daemon", .requireArgument = NO, .priority = LOW, .method = terminal_methods::enableDaemonMode }
};



void manageArguments( int argc, const char **argv)
{
	//saving arguments.
	std::vector<argumentStruct> argumentsList = {};
	bool isValue = false;
	for ( int i = 1; i < argc; i++ )
	{
		if ( !checkConflicts( argv[i] ) && !isValue )
		{
			throwMessage( "Unkown argument [" + std::string(argv[i]) + "]", THROW_CODE::ERROR );
			exit(1);
		}

		if ( isValue )
		{
			//only non optional arguments values are able to throw from here.
			if ( checkConflicts(argv[i]) && argumentsList.back().requireArgument == REQUIRE_ARGUMENTS_ENUM::YES )
			{
				throwMessage( "[" + argumentsList.back().name + "] requires an argument.", THROW_CODE::ERROR );
				exit(1);
			}
			isValue = false;
			//optional arguments values could be used using this approach if the optional value if not the folowing argument.
			if ( !checkConflicts(argv[i]) )
			{
				argumentsList.back().value = argv[i];
				continue;
			}
		}

		for ( size_t f = 0; f < argumentsAvailable.size(); f++ )
		{
			if ( argumentsAvailable.at(f).name != std::string( argv[i] ) )
				continue;
			switch( argumentsAvailable.at(f).requireArgument )
			{
				case REQUIRE_ARGUMENTS_ENUM::NO:
					argumentsList.push_back( argumentsAvailable.at(f) );
					break;
				case REQUIRE_ARGUMENTS_ENUM::OPTIONAL:
					argumentsList.push_back( argumentsAvailable.at(f) );
					isValue = true;
					break;
				case REQUIRE_ARGUMENTS_ENUM::YES:
					if ( argv[i + 1] == nullptr )
					{
						throwMessage( "[" + argumentsAvailable.at(f).name + "] requires an argument.", THROW_CODE::ERROR );
						exit(1);
					}
					argumentsList.push_back( argumentsAvailable.at(f) );
					isValue = true;
					break;
				default:
					break;
			}

				break;
		}
	}

	bool checkManualConfigFile = []( std::vector<argumentStruct> &argumentsList ) -> bool {
		for ( argumentStruct &item : argumentsList )
			if ( item.name == "--config" )
				return true;
		return false;
	}( argumentsList );

	//read the default config file if the used doesn't specify another from command line
	if ( !checkManualConfigFile )
		if ( std::filesystem::exists(configFile) )
			readConfig( configFile );

	//sorting vector args by priority.
	for ( size_t i = 0; i < argumentsList.size(); i++ )
		for ( size_t f = 0; f < argumentsList.size(); f++ )
			if ( argumentsList[i].priority > argumentsList[f].priority )
				std::swap( argumentsList[i], argumentsList[f] );

	for ( argumentStruct &item : argumentsList )
		item.method( item.value );

}
