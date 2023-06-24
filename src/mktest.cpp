#include "terminalFontStyles.hpp"
#include "makefileRule.hpp"
#include "global.hpp"
#include "mktestInfoOutput.hpp"
#include "manageArguments.hpp"
#include "fileOperations.hpp"
#include "cacheOperations.hpp"

//=========================================>
//LOCAL VARIABLES
//=========================================>
static std::string compileComandsContent = "";
static std::thread makeFileGenerationThread;

//reset al the variables to teir default value
void resetVariables()
{
	#ifdef __linux__
	path = "/tmp";
	#endif

	#ifdef __WIN32
	path = getenv("TMP");
	#endif

	editor = "nano ";
	compiler = "g++";
	debug = "";
	defaultEditor = editor+path+file;
	programsArgs = "";
	modArgs = "//mod-> ";
	reEdit = false;
};




std::string getCodeFlags()
{
	std::vector<std::string> vectorFlags = readFile();
	for ( int i = 0; i < vectorFlags.size(); i++ )
		if ( std::count( vectorFlags.begin(), vectorFlags.end(), vectorFlags.at(i) ) > 1 )
		{
			vectorFlags.erase( vectorFlags.begin() + i );
			i--;
		}

	std::string flags = "";
	for ( std::string &item : vectorFlags )
		if ( &item != &vectorFlags.back() )
			flags += item + " ";
		else
			flags += item;

	if ( generateCompileCommands && flags != compileComandsContent )
	{
		std::fstream compileCommandsFile( path + "/compile_flags.txt", std::ios::in | std::ios::out | std::ios::binary );

		if ( !compileCommandsFile.is_open() )
			compileCommandsFile.open( path + "/compile_flags.txt", std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc );

		if ( !compileCommandsFile.is_open() )
		{
			std::cout << "couldn't generate compile_flags.txt at:[" + path + "]"  << std::endl;
		}

		for ( std::string &item : vectorFlags )
			if ( &item != &vectorFlags.back() )
				compileCommandsFile << item << std::endl;
			else
			 	compileCommandsFile << item;

		compileCommandsFile.close();
	}
	compileComandsContent = flags;
	return flags;
}

//main function for mktest
void mktest()
{
	std::error_code errorCode;
	//creating working directory if doesn't exist.
	if ( !std::filesystem::exists(path) )
		if ( !std::filesystem::create_directory(path, errorCode) )
		{
			throwMessage( "mktest couldn't create the directory at [" + path + "]", THROW_CODE::ERROR );
			std::cout << "ERROR DESCRIPTION: " + errorCode.message() << std::endl;
			return;
		}

	// checking existence of files.
	std::string files = "";
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		files += dir.path().string(), files += " ";

	//if no one c++ file is finded, create some one.
	if ( files.find(".cpp") == std::string::npos ) createNecesaryFiles("test.cpp");

	//if exist a cache file, start to reading it.
	if ( files.find( cacheFileName.substr(1, cacheFileName.length()) ) != std::string::npos ) readCache();

	//reading the source file for flags and arguments in code comment.
	readFile();

	//deletring readed files history, so we can provide future compiler linking
	//flags.
	openedFiles.clear();

	//if the file name doesn't have a slash require for the full path, this will add it to the start of the string.
	if ( file.find('/') == std::string::npos ) file =  '/' + file;

	//this is the full command to start editing the file
	defaultEditor = editor + path + file;

	//changing working path, so editors line vim or emacs can view another files in the same sesion.
	const std::string currentPath = std::filesystem::current_path().string();
	std::filesystem::current_path( path );

	std::string flags = "";

	//if this is activated, mktest will generate and update the makefile for your code while you are codding.
	bool generating = generateMakefileWhileCodding;
	if ( generateMakefileWhileCodding )
		makeFileGenerationThread = std::thread( [&](){
			while ( generating )
			{
				flags = getCodeFlags();
				generateMakeFile(flags);
			}
		} );	

	//opening test.cpp file or some one else for editing.
	system( defaultEditor.c_str() );

	if ( generateMakefileWhileCodding )
	{
		generating = false;
		if ( makeFileGenerationThread.joinable() )
			makeFileGenerationThread.detach();
	}

	std::filesystem::current_path(currentPath);

	//reading again the source file for flags and arguments in code comment.
	flags = getCodeFlags();
	
	//showing parameters for the program
	std::cout << TEXT_YELLOW << TEXT_BOLD << "flags: " << TEXT_RESET << flags << std::endl; // showing flags
	std::cout << TEXT_YELLOW << TEXT_BOLD << "args: " << TEXT_RESET << programsArgs << std::endl; // showing argument for your program
	std::cout << TEXT_YELLOW << TEXT_BOLD << "CC: " << TEXT_RESET << compiler << std::endl; // showing compiler to use

	//showing optional information.
	if ( verbose )
		std::cout << verboseOutput;

	//creating MakeFile
	generateMakeFile(flags);
	
	//start to compile the source code and then execute the program
	system( std::string( "make -s -C " + path).c_str() );

	if ( debug != "" )
		system( debug.c_str() );
	else
	{
		std::string execute = path + executable + " " + programsArgs;
		system( execute.c_str() );
	}

	//if reEdit is true, mktest will wait for 3 secconds to let you see the ouput of your program and then will enter again to the editor
	if ( reEdit )
	{
		flags = "";
		std::this_thread::sleep_for( std::chrono::seconds(3));
		resetVariables();
		std::cout << std::endl;
		mktest();
	}

	if ( makeFileGenerationThread.joinable() )
		makeFileGenerationThread.join();
};

int main(int argc, char const *argv[])
{
	manageArguments( argc, argv);


	if ( verbose )
		verboseOutput = std::string(TEXT_BOLD) + std::string(TEXT_CYAN) + "PROJECT FOLDER:" + std::string(TEXT_RESET) + path + "\n" + verboseOutput;

	mktest();

	return 0;
}
