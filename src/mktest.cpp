#include "defaultIncludes.hpp"
#include "templates.hpp"
#include "qt6Templates.hpp"
#include "resources.hpp"
#include "terminalFontStyles.hpp"
#include "makefileRule.hpp"
#include "global.hpp"
#include "configParser.hpp"
#include "testScriptMethods.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::istringstream;
using std::getline;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::istreambuf_iterator;

//=========================================>
//LOCAL VARIABLES
//=========================================>
static bool verbose = false;
static bool customCompileArgs = false;
static string customCompileArgsString = "";
static string verboseOutput = "";

//======================================================================================>
// GLOBAL VARIALBES
//======================================================================================>


//The openedFiles vector is by the momment only used by a lamba function to store a history
//of readed files that must ignore to read twice or more!.
//The function is called recursiveLecture( string objective ).
vector<string> openedFiles;

//======================================================================================>

void help()
{
	cout << TEXT_GREEN << TEXT_BOLD "mktest 2.1" << TEXT_RESET << endl;
	cout << "	simple tool for faster c++ project and test on terminal" << endl << endl;

	cout << TEXT_GREEN << TEXT_BOLD << "USAGE:" << TEXT_RESET << endl;
	cout << "	mktest" << TEXT_BLUE << TEXT_BOLD << " -> " << TEXT_RESET << "for it's own create a test.cpp file with a Makefile." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --new <name> " << TEXT_BLUE << "-> " << TEXT_RESET << "create a new file with a optional name." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --keep " << TEXT_BLUE << "-> " << TEXT_RESET << "if you create a new file, when you put this" << endl;
	cout << "			 flag, it will write a comment modArg at the" << endl;
	cout << "			 first line of your code." << endl;
	cout << TEXT_BOLD << TEXT_YELLOW << "			 [" << TEXT_RESET << " see MODARGS for more info " << TEXT_BOLD << TEXT_YELLOW << "]" << TEXT_RESET << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --args <args> " << TEXT_BLUE << "-> " << TEXT_RESET << "pass args to your program." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --editor <editor> " << TEXT_BLUE << "-> " << TEXT_RESET << "change the editor to edit the code." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --config <file> " << TEXT_BLUE << "-> " << TEXT_RESET << "use the specified file as config file." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --saveProj <folder> " << TEXT_BLUE << "-> " << TEXT_RESET << "save all your code in the optional" << endl;
	cout << "			 	      folder." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --clearCache " << TEXT_BLUE << "-> " << TEXT_RESET << "delete the cache file." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --setEnv <environment> " << TEXT_BLUE << "-> " << TEXT_RESET << "use an predefined development" << endl;
	cout << "					 environment from a config file." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --verbose " << TEXT_BLUE << "-> " << TEXT_RESET << "display detailed information." << endl;
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " --template <template> " << TEXT_BLUE << "-> " << TEXT_RESET << "create a code file with a" << endl;
	cout << "			  		predefined example of code." << endl;
	cout << TEXT_BOLD << TEXT_GREEN << "TEMPLATES AVAILABLE:" << TEXT_RESET << endl;
	cout << "	sdl2, sdl2_image, curl, imlib2, xlib, qt6." << endl << endl;

	cout << TEXT_BOLD << TEXT_GREEN << "MODARGS:" << TEXT_RESET << endl;
	cout << "	A modarg is a flag that is located in some part of your code" << endl;
	cout << "	as a custom comentary of c/c++ to change the behaviour of" << endl;
	cout << "	mktest at the time to handle with your code file." << endl << endl;

	cout << TEXT_BOLD << TEXT_CYAN << "sintax:" << TEXT_RESET << " //mod-> <modargs>" << endl << endl;
	cout << TEXT_BOLD << TEXT_CYAN << "modargs without value:" << TEXT_RESET << endl << endl;
	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "keep " << TEXT_BLUE << "-> " << TEXT_RESET << "remember the current file as the default to edit" << endl;
	cout << "			the next time you execute mktest." << endl;
	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "reEdit " << TEXT_BLUE << "-> " << TEXT_RESET << "compile and execute your program and wait 3" << endl;
	cout << "			  seconds to let you see the output and enter to" << endl;
	cout << "			  edit your code again." << endl;
	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "debug " << TEXT_BLUE << "-> " << TEXT_RESET << "enter to debug your executable with gdb rather" << endl;
	cout << "			 than run it." << endl << endl;

	cout << TEXT_BOLD << TEXT_CYAN << "modargs with value:" << TEXT_RESET << endl << endl;

	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "c++-> <version> " << TEXT_BLUE << "-> " << TEXT_RESET << "change the version of c++ to use." << endl;
	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "editor-> <editor> " << TEXT_BLUE << "-> " << TEXT_RESET << "chage the code editor to use." << endl;
	cout << "	//mod-> " << TEXT_BOLD << TEXT_YELLOW << "compiler-> <compiler> " << TEXT_BLUE << "-> " << TEXT_RESET << "change the compiler to use." << endl << endl;

	cout << TEXT_BOLD << TEXT_GREEN << "PROGRAM ARGUMENTS:" << TEXT_RESET << endl;

	cout << "	another way to pass argument to your program is to use another" << endl;
	cout << "	custom comentary of c/c++." << endl << endl;

	cout << TEXT_BOLD << TEXT_CYAN << "syntax: " << TEXT_RESET << "//args-> foo* foo*" << endl;
}

bool FileExist( string name )
{
	if ( name.find('/') == string::npos )
		name = "/" + name;

	std::filesystem::path file( path + name );
	auto status = std::filesystem::symlink_status( file );

	if ( std::filesystem::exists( status ) )
		return true;
	return false;
}

vector<string> getListFiles( string mainFilePath )
{
    std::ifstream file( mainFilePath );
    vector<string> ret;

    while ( file.good() )
    {
        string element = "";
        file >> element;

        if ( element == "#include" )
        {
            file >> element;
            if ( element.find('"') != string::npos && element.find(".h") != string::npos )
            {
            	//clear quotation marks from the string to get the normal name
            	element = element.substr( 1, element.length() -2 );
            	
            	if ( FileExist(element) )
            	{	
            		//if the element file anm isn't in the file vector, add it,
            		if ( std::count( ret.begin(), ret.end(), element ) < 1 )
            			ret.push_back(element);

            		//adding linked files.
            		
            		vector aux = getListFiles( path + "/" + element);
            		//cout << "vectorAux: " << aux.back() << endl;
            		for ( auto files : aux )
            			if ( std::count( ret.begin(), ret.end(), files ) < 1 )
            				ret.push_back(files);
            	}

            	//checking if there's a source file
                element = element.substr( 0, element.find(".h") ) + ".cpp";
                
                if ( FileExist(element) )
                {
                	//adding file if not exist. 
                	if ( std::count( ret.begin(), ret.end(), element ) < 1 )
                		ret.push_back(element);
                	
                	//reading for files linked.
                	vector aux = getListFiles( path + "/" + element );
                	for( auto files : aux )
                		if ( std::count( ret.begin(), ret.end(), files ) < 1 )
                			ret.push_back(files);
                }
            }
        }
    }

    return ret;
}

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

//argument conflict handler
bool checkConflicts( string arg )
{
	string data[] = { "--new", "--help", "--args", "--editor", "--saveProj", "--template", "clearCache", "--keep", "--standAlone", "--setEnv", "--verbose", "--config" };

	for ( auto args : data )
		if ( arg == args )
			return true;
	return false;
}

//error handler
void throwError( string wrongArg, string flag, string whitchError )
{
	if( whitchError == "invalid" )
		cout << TEXT_RED << TEXT_BOLD << "ERROR: " << TEXT_RESET << " [ " << wrongArg << " ] is not a valid argument for " << flag << endl;

	if ( whitchError == "require" )
		cout << TEXT_RED << TEXT_BOLD << "ERROR: " << TEXT_RESET << flag << " require an argument" << endl;

	exit(1);
}

//file creation handler
void createNecesaryFiles(string fileName)
{
	//scan the path of your file for a previous file with the same name of the new
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		if ( dir.path().string().find(fileName) != string::npos )
		{
			string aux = "";

			//report the problem to the user and ask for what should do.
			cout << TEXT_UNDERLINE << TEXT_BOLD << BG_YELLOW << TEXT_BG_BLACK_BRIGHT_YELLOW << "WARNING:" << TEXT_RESET << " [ " << fileName << " ] this file already exist." << endl;
			cout << "What should we do? [replace/edit/none]: ";
			std::cin >> aux;
			if ( aux.find('r') != string::npos )
			{
				cout << "replacing" << endl;
				break;
			}
			else if ( aux.find('e') != string::npos )
			{
				file = fileName;
				return;
			}
			else
			{
				cout << "aborting" << endl;
				exit(0);
			}
		}

	//where create the file and stream to the file
	string name = path + '/' + fileName;
	ofstream saveFile (name);

	//if the new file is a *.cpp file create a c++ template
	if ( fileName.find(".cpp") != string::npos )
	{
		if ( modArgs != "//mod->" )
			saveFile << modArgs << endl;

		saveFile << "//cpp template\n";
		saveFile << "#include <iostream>\n";
		saveFile << "#include <string>\n\n";
		saveFile << "using std::string;\n";
		saveFile << "using std::cout;\n";
		saveFile << "using std::endl;\n\n";
		saveFile << "int main(int argc, char *argv[])\n{\n\n return 0;\n}";
	}
	else
	//if the new file is a header file, create a header template
	if ( fileName.find(".h") != string::npos )
	{
		string define = "";
		for ( int i = 0; i < fileName.length(); i++ )
			define.push_back( fileName[i] == '.' ? '_' : toupper( fileName[i] ) );
		saveFile << "#ifndef " << define << endl;
		saveFile << "#define " << define << endl << endl;
		saveFile << "//code here" << endl << endl;
		saveFile << "#endif";
	}
	else
		//if no is a kown type of file just print the name of the file inside that file
		saveFile << "//" + fileName;
	saveFile.close();

	//set the file as default for edit
	file = fileName;
};

//read the current cache file
void readCache()
{
	//stream from cache file
	ifstream cacheFile( path + cacheFileName );

	//read the chache file
	while( cacheFile.good() )
	{
		string tag;
		cacheFile >> tag;

		//aply any change of the cache file to the mktest program
		if ( tag == "fileToEdit:" )
			file.clear(),
			cacheFile >> file;
	}
}

//this generate/write a cache file to remeber changes tempolary
void writeCache(string whichTag, string value)
{
	//string where the content of the directory will be saved.
	string search = "";

	//string where the cache info will be saved before be writen in the cache file
	string info = "";

	//read directory to find what files are there.
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		search += dir.path().string() + " ";

	//check if there's a previous cache file.
	if ( search.find( cacheFileName.substr(1, cacheFileName.length()) ) != string::npos )
	{
		//stream from the previous cache file
		ifstream reading( path + cacheFileName );

		//pass the information of that cache file to the [ info ] string
		//and make the required changes acording to the function argumets
		while ( reading.good() )
		{
			//passing the information of the stream to a string word by word
			string tag;
			reading >> tag;

			//if the previous cache file has a tag equal to what you want
			//to save/edit, remplace that with the new value
			if ( tag == whichTag )
			{
				info += whichTag + " ";
				info += ( value.find('/') != string::npos ? value.substr(1, value.length()) : value );
				whichTag.clear();
				value.clear();
			}
		}

		//if the value is not empty that means that in the cache the tag was never declared before
		//and proceed to write
		if ( value != "" )
		{
			if ( info == "" )
			{
				info = whichTag + " ";
				info += ( value.find('/') != string::npos ? value.substr(1, value.length()) : value );
			}
			else
			{
				info += '\n' + whichTag + " ";
				info += ( value.find('/') != string::npos ? value.substr(1, value.length()) : value );
			}
		}

		//close the data stream
		reading.close();
	}
	else
		//if a previus cache file doesn't exist, just write the desired values on a new file
		info = whichTag + " " + value;

	//create a new cache file with the desired information.
	ofstream writing( path + cacheFileName );

	//write the data.
	writing << info;

	//close the stream.
	writing.close();
}

//if the file exist delete the file
void deleteFile(string File)
{
	string search;

	//read the directory to get te files in there.
	for ( auto &entry : std::filesystem::directory_iterator( path ) )
		search += entry.path().string(), search += " ";

	//if the name of the file contain a slash, this will remove that to find the name on the search string
	string name = File.find('/') != string::npos ? File.substr( 1, File.length() ) : File;

	//if the file exist, delete it, if not, report to the user.
	if ( search.find( name ) != string::npos )
		std::filesystem::remove( path + File );
	else
		cout << TEXT_RED << TEXT_BOLD << "ERROR:"  << TEXT_RESET << endl << path + File << " doesn't exist yet" << endl;
		exit(1);
}

//read your code file for comment flags and required argument to compile
string readFile( string *recursivePreviousFlags = nullptr )
{
	//reset some variables and values that change again if their are nedded.
	if ( reEdit ) reEdit = false;
	writeCache("fileToEdit", "test.cpp");
	debug = "";

	//stream from your code file
	ifstream reading(path+file);

	//buffer to read the whole lines and a result for return the nedded flags for compile your code
	string buffer = "", results = recursivePreviousFlags != nullptr ? *recursivePreviousFlags : "";
	if ( customCompileArgs )
		results = customCompileArgsString;
	
	while ( getline( reading, buffer ) )
	{
		//detecting mod args
		if ( buffer.find("//mod-> ") != string::npos )
		{
			//the kind of editor if was specified, if isn't specified by default will use nano
			if ( buffer.find(" editor-> ") != string::npos )
			{	
				if ( buffer.find(" emacs") != string::npos ) editor = "emacs ";
				if ( buffer.find(" nvim") != string::npos ) editor = "nvim ";
				if ( buffer.find(" vim") != string::npos ) editor = "vim ";
				if ( buffer.find(" vi") != string::npos ) editor = "vi ";
			};

			//the compiler to use, if isn't specified by default will use g++
			if( buffer.find( " compiler-> " ) != string::npos )
			{
				if ( buffer.find(" gcc") != string::npos ) compiler = "gcc";
				if ( buffer.find(" g++") != string::npos ) compiler = "g++";
				if ( buffer.find(" clang") != string::npos ) compiler = "clang";
				if ( buffer.find(" clang++") != string::npos ) compiler = "clang++";
			};

			 //coplile the code with debug flags and open the executable with gdb for debug
			if ( buffer.find(" debug") != string::npos )
			{ 
				results += "-g -O3 "; debug = "	gdb --se=" + path + executable + " --readnow -q\0";
			};

			if ( buffer.find( " keep") != string::npos ) writeCache("fileToEdit: ", file);

			//active the [ reEdit ] variable for not get out from the mktest program
			if ( buffer.find(" reEdit") != string::npos ) reEdit = true;

			//use language version
			if ( buffer.find(" c++->") != string::npos && !customCompileArgs )
			{
				if ( buffer.find( " 17" ) != string::npos ) results += "-std=c++17 ";
			}
		};

		//to give arguments for your executable, see mktest --help for more info
		if ( buffer.find( "//args-> " ) != string::npos )
		{
			programsArgs = "";
			for ( int i = 9; i < buffer.size(); i++ )
				programsArgs+= buffer[i];
		};

		//detecting libraries and headers,
		if ( !customCompileArgs )
		if ( buffer.find("#include") != string::npos )
		{
			//for compiler autocompletion flags
			#if defined(__WIN32)
				if ( buffer.find("SDL.h") != string::npos ) results += "-lmingw32 -lSDL2main -lSDL2 ";
			#else
				if ( buffer.find("SDL.h") != string::npos ) results += "-lSDL2 ";
			#endif
			if ( buffer.find("SDL_image.h") != string::npos ) results += "-lSDL2_image ";
			if ( buffer.find("Xlib.h") != string::npos ) results += "-lX11 ";
			if ( buffer.find("curl.h") != string::npos ) results += "-lcurl ";
			if ( buffer.find("Imlib2.h") != string::npos ) results += "-lImlib2 ";
			if ( buffer.find("SDL_mixer.h") != string::npos) results += "-lSDL2_mixer ";
			if ( buffer.find("SDL2_gfxPrimitives.h") != string::npos ) results += "-lSDL2_gfx ";
			if ( buffer.find("SDL_ttf.h") != string::npos  ) results += "-lSDL2_ttf ";
			if ( buffer.find( "Q" ) != string::npos || buffer.find("q") != string::npos )
			{
				auto addMissingLibs = [&]( string libs ){ 
					if ( results.find("-lQt6Core") == string::npos )
							results += "-lQt6Core  ";
					if ( results.find("-std=c++17") == string::npos )
						results += "-std=c++17 ";
					if ( results.find(libs) == string::npos )
						results += libs + " ";
				};
				for ( int i = 0; i < 322; i++ )
				{
					if ( buffer.find( Qt6::widgetsHeaders[i] ) != string::npos )
					{
						addMissingLibs("-lQt6Widgets");
						break;
					}
				}
				for ( int i = 0; i < 24; i++ )
				{
					if ( buffer.find(Qt6::concurrentHeaders[i]) != string::npos )
					{
						addMissingLibs("-lQt6Concurrent");	
						break;
					}
				}

				for ( int i = 0; i < 341; i++ )
				{
					if ( buffer.find( Qt6::guiHeaders[i] ) != string::npos )
					{
						addMissingLibs("-lQt6Gui");
						break;
					}
				}
			}

			//for MakeFile rules
			if ( buffer.find('"') != string::npos )
			{	
				//cleaning string for have only the file name
				buffer = buffer.substr( buffer.find('"') + 1, buffer.length() );
				buffer = buffer.substr(0, buffer.find('"') );

				if ( buffer.find(".h") != string::npos )
				{ 
					//This lamda function allows to read recursively all the file linked to our test.cpp file
					//and it's tree hierarchy.
					auto recursiveLecture = [&]( std::string &objective ) {
						for ( string &item : openedFiles )
							if ( path + "/" + objective == item )
								return;
						openedFiles.push_back( path + "/" + objective );
						string oldFile = file;
						file = "/" + objective;
						results = readFile( &results );
						file = oldFile;
					};

					if ( FileExist( buffer ) )
					{
						//recursive reading to get includes.
						recursiveLecture( buffer );

						//adding Makefile rule to the vector of Makefile rules
						mkfileRules.push_back( makefileRule( buffer + ".gch" , buffer, "",compiler ) );

						//check repetitions
					 	if ( mkfileRules.size() > 1 )
					 		for ( int i = 0; i < mkfileRules.size() -1; i++ )
					 			if ( mkfileRules.at(i).provideGoal() == mkfileRules.back().provideGoal() )
					 				mkfileRules.pop_back();
					 }

					 //cheching if there's a source file for the header and adding it if exist.
					 string source = buffer.substr(0, buffer.find(".h")) + ".cpp";
					 if ( FileExist( source ) )
					 {
						 //recursive reading to get includes.
						 recursiveLecture( source );

					 	source = source.substr( 0, source.find(".cpp") );
					 	mkfileRules.push_back( makefileRule( source + ".o", source + ".cpp", "", compiler + " -c " ) );

					 	if ( mkfileRules.size() > 1 )
					 		for ( int i = 0; i < mkfileRules.size() -1; i++ )
					 			if ( mkfileRules.at(i).provideGoal() == mkfileRules.back().provideGoal() )
					 				mkfileRules.pop_back();
					 		
					 }
				}
			}
		};
	};

	if ( reading.is_open() )
		reading.close();

	return results;
};

//creating required Makefile with all the required arguments, flags and files for build
void makeFile(string flags)
{
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

//this funcion create a template of a predefined type of project for some libraries
void createExampleFile(string wich)
{
	//if the working directory doesn't exist, create one.
	if ( !std::filesystem::exists(path) )
		std::filesystem::create_directory(path);

	//delete the previous content of the directory
	for ( auto &entry : std::filesystem::directory_iterator(path) )
		std::filesystem::remove_all(entry.path());

	//stream for the current code file
	ofstream codeFile( path + file, std::ios::out | std::ios::binary );

	//templates availables
	if( wich == "curl" ){ codeFile << curlExample; codeFile.close(); return; };
	if( wich == "imlib2" ){ codeFile << imlib2Template; codeFile.close(); return; };
	if( wich == "xlib" ){ codeFile << xlibTemplate; codeFile.close(); return; };
	if ( wich == "sdl2" ){ codeFile << sdl2_example; codeFile.close(); return; }
	if( wich == "sdl2_image" )
	{
		ofstream imageData ( path +"/image.jpg", std::ios::out | std::ios::binary);
		for ( auto i : imageForSDl2_ImageTemplate )
			imageData << i;
		imageData.close();
		codeFile << sdl2_ImageExample; codeFile.close();
		return;
	};

	if ( wich == "qt6" )
	{
		codeFile << Qt6::main_cpp;
		codeFile.close();
		ofstream file( path + "/window.hpp", std::ios::out | std::ios::binary );
		file << Qt6::window_hpp;
		file.close();
		file = ofstream( path + "/window.cpp", std::ios::out | std::ios::binary );
		file << Qt6::window_cpp;
		file.close();
		return;
	}

	//close stream.
	codeFile.close();

	//report error to the user.
	throwError(wich, "--template", "invalid");
	exit(1);
};

//save your code file on the provided path
void saveCode( string onThisPlace = home + "/mktestProj" )
{
	if ( onThisPlace == "." )
		onThisPlace = "./mktestProj";
		
		//checking if the destination folder exist.
		std::filesystem::path where( onThisPlace );
		auto status = std::filesystem::symlink_status( where );

		//if not exist create one.
		if ( !std::filesystem::exists( status ) )
			std::filesystem::create_directory( where );

	//if the path provided is not a dot, and the last character is a slash, remove that character.
	if ( onThisPlace.size() > 1 && onThisPlace[onThisPlace.size() -1] != '/' )
		onThisPlace.push_back('/');

	//get the list of all files that are related in some way with your project
	vector<string> filesToRead = getListFiles( path + file );
	filesToRead.push_back(file);
	filesToRead.push_back( "Makefile" );

	//knowing that the [ file ] variable has an slash, it was necesary to remove it from the [ onThisPlace ] variable if exist
	

	//saving any linked file of your project
	for ( auto files : filesToRead )
	{
		ofstream fileToSave( string( onThisPlace + files ) );
		ifstream codeFile( path + "/" + files );
	
		//the rdbuf() method allow to copy all the file content once,
		fileToSave << codeFile.rdbuf();

		fileToSave.close();
		codeFile.close();

	}

	//report path to the user
	cout << TEXT_YELLOW << TEXT_BOLD << "save on: " << onThisPlace << TEXT_RESET << endl;

	exit(0);
}

//main function for mktest
void mktest()
{
	std::error_code errorCode;
	//creating working directory if doesn't exist.
	if ( !std::filesystem::exists(path) )
		if ( !std::filesystem::create_directory(path, errorCode) )
		{
			cout << "ERROR: mktest couldn't create the directory at [" + path + "]\n";
			cout << "ERROR DESCRIPTION: " + errorCode.message() << endl;
			return;
		}

	// checking existence of files.
	string files = "";
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		files += dir.path().string(), files += " ";

	//if no one c++ file is finded, create some one.
	if ( files.find(".cpp") == std::string::npos ) createNecesaryFiles("test.cpp");

	//if exist a cache file, start to reading it.
	if ( files.find( cacheFileName.substr(1, cacheFileName.length()) ) != string::npos ) readCache();

	//reading the source file for flags and arguments in code comment.
	readFile();

	//deletring readed files history, so we can provide future compiler linking
	//flags.
	openedFiles.clear();

	//if the file name doesn't have a slash require for the full path, this will add it to the start of the string.
	if ( file.find('/') == string::npos ) file =  '/' + file;

	//this is the full command to start editing the file
	defaultEditor = editor + path + file;

	//changing working path, so editors line vim or emacs can view another files in the same sesion.
	const string currentPath = std::filesystem::current_path().string();
	std::filesystem::current_path( path );

	 //opening test.cpp file or some one else for editing.
	system( defaultEditor.c_str() );

	std::filesystem::current_path(currentPath);

	//reading again the source file for flags and arguments in code comment.
	string flags = readFile();
	
	//showing parameters for the program
	cout << TEXT_YELLOW << TEXT_BOLD << "flags: " << TEXT_RESET << flags << endl; // showing flags
	cout << TEXT_YELLOW << TEXT_BOLD << "args: " << TEXT_RESET << programsArgs << endl; // showing argument for your program
	cout << TEXT_YELLOW << TEXT_BOLD << "CC: " << TEXT_RESET << compiler << endl; // showing compiler to use

	//showing optional information.
	if ( verbose )
		cout << verboseOutput;

	//creating MakeFile
	makeFile(flags);
	
	//start to compile the source code and then execute the program
	system( string( "make -s -C " + path).c_str() );

	if ( debug != "" )
		system( debug.c_str() );
	else
	{
		string execute = path + executable + " " + programsArgs;
		system( execute.c_str() );
	}

	//if reEdit is true, mktest will wait for 3 secconds to let you see the ouput of your program and then will enter again to the editor
	if ( reEdit )
	{
		flags = "";
		sleep_for(seconds(3));
		resetVariables();
		cout << endl;
		mktest();
	}
};

int main(int argc, char const *argv[])
{
	for ( int i = 0; i < argc; i++ )
		if ( string(argv[i]) == "--config" && argv[i+1] != nullptr )
		{
			if ( checkConflicts( string( argv[i+1] ) ) )
			{
				throwError( string( argv[i+1] ), string(argv[i]), "invalid" );
				break;
			}

			string fileName = argv[i+1];

			if ( !std::filesystem::exists( fileName ) )
			{
				cout << "ERROR: config file [" + fileName + "] doesn't exist!" << endl;
				break;
			}
				configFile = fileName;
		}
	//read config file.
	if ( std::filesystem::exists(configFile) )
		readConfig( configFile );

	//arguments management
	if ( argc > 1 )
	{
		for ( int i = 1; i < argc; i++ )
		{
			string aux = argv[i];

			if ( aux == "--new" )
			{
				if ( argv[i + 1] != nullptr ) aux = argv[i+1];

				checkConflicts(aux) ? createNecesaryFiles("test.cpp") : createNecesaryFiles(aux);		
			}

			if ( aux == "--args" )
			{
				for ( int f = i + 1; f < argc; f++ )
					if ( !checkConflicts( string( argv[f] ) ) )
					programsArgs += string( argv[f] ) + " ";
			};

			if ( aux == "--keep" )
			{
				modArgs += " keep";
				if ( file != "/test.cpp" )
				{
					//if the [ file ] variable has an slash character, is nedded to romove that
					string read = file.find('/') != string::npos ? file.substr(1, file.length() ) : file;

					//search for the code file
					for ( auto dir : std::filesystem::directory_iterator( path ) )
						if ( dir.path().string().find( read ) != string::npos )
						{
							//stream to read the code file.
							ifstream ReadcodeFile( path + "/" + read );
							string buffer = "";

							//stracting the information from the code file.
							istreambuf_iterator<char> begin( ReadcodeFile ), end;
							while ( begin != end )
								buffer += *begin, begin++;

							ReadcodeFile.close();

							//stream to rewrite the file with the mod arg [ keep ].
							ofstream writeCodeFile( path + "/" + read );

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
					if ( !checkConflicts( string( argv[i + 1] ) ) )
						editor = string( argv[i + 1] )  + " ";
					else
						throwError( string( argv[i+1] ) , aux, "invalid");
				else
					throwError( "", aux, "require");
			}

			if ( aux == "--saveProj" )
			{
				if ( argv[i+1] != nullptr )
					if ( !checkConflicts( string( argv[i+1] ) ) )
						saveCode( string( argv[i+1] ) );
					else
						throwError( string(argv[i+1]) , aux, "invalid");
				else
					saveCode();
				exit(0);
			}

			if ( aux == "--template" )
			{
				if ( argv[ i + 1 ] != nullptr )
					if ( !checkConflicts( string( argv[i+1] ) ) )
						createExampleFile( string( argv[i+1] ) );
					else
						throwError( string( argv[i+1] ) , aux, "invalid");
				else
					throwError( "" , "--template", "require" );
			}

			if ( aux == "--clearCache" ) deleteFile( cacheFileName );
			//if ( aux == "--standAlone" )

			if ( aux == "--setEnv" )
			{
				if ( argv[i + 1] != nullptr )
					if ( !checkConflicts( string( argv[i + 1] ) ) )
					{
						if ( setEnvironment( string( argv[i + 1] ) ) )
						{
							string color = string(TEXT_BOLD) + string(TEXT_CYAN);
							verboseOutput += color;
							verboseOutput += "enviroment: ";
							verboseOutput += TEXT_RESET;
							verboseOutput += string( argv[i + 1 ] ) + "\n";

							verboseOutput += color + "LD_LIBRARY_PATH:" + string(TEXT_RESET) + string( std::getenv("LD_LIBRARY_PATH") != nullptr ? std::getenv("LD_LIBRARY_PATH") : "" ) + "\n";
							verboseOutput += color + "LIBRARY_PATH:" + string(TEXT_RESET) + string( std::getenv("LIBRARY_PATH") != nullptr ? std::getenv("LIBRARY_PATH") : "" ) + "\n";
							verboseOutput += color + "CPATH:" + string(TEXT_RESET) + string( std::getenv("CPATH") != nullptr ? std::getenv("CPATH") : "" ) + "\n";
							for ( auto item : declaredEnvironments )
								if ( item.name == string( argv[i + 1] ) )
								{
									if ( !item.paths.empty() )
										verboseOutput += color + "PATH:" + string(TEXT_RESET) + string( std::getenv("PATH") != nullptr ? std::getenv("PATH") : "" ) + "\n";
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
			}

			if ( aux == "--compileCommands" )
			{
				if ( argv[i + 1] != nullptr )
					if ( !checkConflicts( string( argv[i + 1] ) ) )
					{
						customCompileArgs = true;
						customCompileArgsString = string( argv[i + 1] );
					}
					else
						exit(1);
			}

			if ( aux == "--verbose" )
				verbose = true;
		};
	};

	if ( verbose )
		verboseOutput = string(TEXT_BOLD) + string(TEXT_CYAN) + "PROJECT FOLDER:" + string(TEXT_RESET) + path + "\n" + verboseOutput;

	mktest();

	return 0;
}
