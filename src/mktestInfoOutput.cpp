#include "defaultIncludes.hpp"
#include "terminalFontStyles.hpp"

void help()
{
	using namespace std;
	cout << TEXT_GREEN << TEXT_BOLD "mktest 3.0" << TEXT_RESET << endl;
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
	cout << "	mktest" << TEXT_BOLD << TEXT_YELLOW << " -v " << TEXT_BLUE << "-> " << TEXT_RESET << "show program version and platform." << endl;
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

//error handler
void throwError( std::string wrongArg, std::string flag, std::string whitchError )
{
	if( whitchError == "invalid" )
		std::cout << TEXT_RED << TEXT_BOLD << "ERROR: " << TEXT_RESET << " [ " << wrongArg << " ] is not a valid argument for " << flag << std::endl;

	if ( whitchError == "require" )
		std::cout << TEXT_RED << TEXT_BOLD << "ERROR: " << TEXT_RESET << flag << " require an argument" << std::endl;

	exit(1);
}
