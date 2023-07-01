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
	std::string data[] = { "--new", "--help", "--args", "--editor", "--saveProj", "--template", "clearCache", "--keep", "--standAlone", "--setEnv", "--verbose", "--config", "--compileCommands", "-v", "--path", "--daemon" };

	for ( auto args : data )
		if ( arg == args )
			return true;
	return false;
}

void manageArguments( int argc, const char **argv)
{
	for ( int i = 0; i < argc; i++ )
		if ( std::string( argv[i] ) == "-v" )
		{
			std::cout << version << std::endl;
			exit(0);
		}

	for ( int i = 0; i < argc; i++ )
		if ( std::string( argv[i] ) == "--path" )
		{
			if ( argv[i + 1] == nullptr )
			{
				throwMessage( "[--path] requires an argument", THROW_CODE::ERROR );
				exit(1);
			}

			std::string newPath = argv[i + 1];

			if ( checkConflicts( newPath ) )
			{
				throwMessage( "[" + newPath + "] is not a valid argument for [--path]" , THROW_CODE::ERROR);
				exit(0);
			}
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
			break;
		}

	for ( int i = 0; i < argc; i++ )
		if ( std::string(argv[i]) == "--config" && argv[i+1] != nullptr )
		{
			if ( checkConflicts( std::string( argv[i+1] ) ) )
			{
				throwError( std::string( argv[i+1] ), std::string(argv[i]), "invalid" );
				break;
			}

			std::string fileName = argv[i+1];

			if ( !std::filesystem::exists( fileName ) )
			{
				std::cout << "ERROR: config file [" + fileName + "] doesn't exist!" << std::endl;
				break;
			}
				configFile = fileName;
			break;
		}
	//read config file.
	if ( std::filesystem::exists(configFile) )
		readConfig( configFile );

	//arguments management
	if ( argc > 1 )
	{
		for ( int i = 1; i < argc; i++ )
		{
			std::string aux = argv[i];

			if ( aux == "--new" )
			{
				if ( !std::filesystem::exists(path) )
					std::filesystem::create_directory(path);
				if ( !std::filesystem::exists(path) )
				{
					throwMessage("could not create the main working directory at: " + path, THROW_CODE::ERROR );
					std::cout << "where: [--new] argument handler position" << std::endl;
					exit(1);
				}

				if ( argv[i + 1] != nullptr ) aux = argv[i+1];


				!checkConflicts(aux) ? createNecesaryFiles(aux) : createNecesaryFiles("test.cpp");
			}

			if ( aux == "--args" )
			{
				for ( int f = i + 1; f < argc; f++ )
					if ( !checkConflicts( std::string( argv[f] ) ) )
					programsArgs += std::string( argv[f] ) + " ";
			};

			if ( aux == "--keep" )
			{
				if ( !std::filesystem::exists(path) )
				{
					throwMessage( "not working directoty to keep file", THROW_CODE::WARNING );
					continue;
				}
				modArgs += " keep";
				if ( file != "/test.cpp" )
				{
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
			}

			if ( aux == "--help" )
			{
				help();
				exit(0);
			}

			if ( aux == "--editor" )
			{
				if ( argv[i + 1] != nullptr )
					if ( !checkConflicts( std::string( argv[i + 1] ) ) )
						editor = std::string( argv[i + 1] )  + " ";
					else
						throwError( std::string( argv[i+1] ) , aux, "invalid");
				else
					throwError( "", aux, "require");
			}

			if ( aux == "--saveProj" )
			{
				if ( argv[i+1] != nullptr )
					if ( !checkConflicts( std::string( argv[i+1] ) ) )
						saveCode( std::string( argv[i+1] ) );
					else
						throwError( std::string(argv[i+1]) , aux, "invalid");
				else
					saveCode();
				exit(0);
			}

			if ( aux == "--template" )
			{
				if ( argv[ i + 1 ] != nullptr )
					if ( !checkConflicts( std::string( argv[i+1] ) ) )
						createExampleFile( std::string( argv[i+1] ) );
					else
						throwError( std::string( argv[i+1] ) , aux, "invalid");
				else
					throwError( "" , "--template", "require" );
			}

			if ( aux == "--clearCache" ) deleteFile( cacheFileName );
			//if ( aux == "--standAlone" )

			if ( aux == "--setEnv" )
			{
				if ( argv[i + 1] == nullptr )
					continue;
				if ( checkConflicts( std::string( argv[i + 1] ) ) )
				{
					throwError( std::string( argv[i+1] ), aux, "invalid");
					exit(1);
				}
				
				if ( setEnvironment( std::string( argv[i + 1] ) ) )
				{
					std::string color = std::string(TEXT_BOLD) + std::string(TEXT_CYAN);
					verboseOutput += color;
					verboseOutput += "enviroment: ";
					verboseOutput += TEXT_RESET;
					verboseOutput += std::string( argv[i + 1 ] ) + "\n";
					verboseOutput += color + "LD_LIBRARY_PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("LD_LIBRARY_PATH") != nullptr ? std::getenv("LD_LIBRARY_PATH") : "" ) + "\n";
					verboseOutput += color + "LIBRARY_PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("LIBRARY_PATH") != nullptr ? std::getenv("LIBRARY_PATH") : "" ) + "\n";
					verboseOutput += color + "CPATH:" + std::string(TEXT_RESET) + std::string( std::getenv("CPATH") != nullptr ? std::getenv("CPATH") : "" ) + "\n";
					for ( auto item : declaredEnvironments )
						if ( item.name == std::string( argv[i + 1] ) )
						{
							if ( !item.paths.empty() )
								verboseOutput += color + "PATH:" + std::string(TEXT_RESET) + std::string( std::getenv("PATH") != nullptr ? std::getenv("PATH") : "" ) + "\n";
							if ( !item.args.empty() )
							{
								customCompileArgs = true;
								customCompileArgsString = item.args;
							}
							break;
						}
				}
				else
					exit(1);
				
			}

			if ( aux == "--compileCommands" )
			{
				if ( argv[i + 1] != nullptr )
				{
					if ( !checkConflicts( std::string( argv[i + 1] ) ) )
					{
						customCompileArgs = true;
						customCompileArgsString = std::string( argv[i + 1] );
					}
					else
						exit(1);
				}
			}

			if ( aux == "--verbose" )
				verbose = true;

			if ( aux == "--daemon" )
				generateMakefileWhileCodding = true;
		};
	};
}
