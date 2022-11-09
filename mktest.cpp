#include "defaultIncludes.h"
#include "templates.h"
#include "resources.h"

#define TEST cout << "test" << endl;

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::istringstream;
using std::getline;

//======================================================================================
// GLOBAL VARIALBES
//======================================================================================

string editor = "nano ";
string path = "/tmp/test.cpp";
string compiler = "g++";
string mkfileExtras = "";
string defaultEditor = editor+path;

//======================================================================================

string locateFile(string path){

	string files = "", aux = "";

	for ( const auto& entry : std::filesystem::directory_iterator( path ) )
	{

		aux = entry.path();

		if ( aux.find(".cpp") != string::npos || aux.find("Makefile") != string::npos )
		{

			files += entry.path();
			files += '\n';

		};

	};

	return files;
};

void createNecesaryFiles(string fileName){

	string name = "/tmp/" + fileName;
	ofstream file (name);
	file << "//cpp template\n";
	file << "#include <iostream>\n\n";
	file << "using std::cout;\n";
	file << "using std::endl;\n\n";
	file << "int main(){\n\n return 0;\n}";
	file.close();

};

string readFile(){

	string lines = "/tmp/test.cpp";
	if (FILE *fp = std::fopen(lines.c_str(), "r"))
	{
		lines = "";
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			lines += buf;
		fclose(fp);
	}

	istringstream d (lines);
	string buffer = "", results = "";
	
	while ( getline( d, buffer ) )
	{

		if ( buffer.find("//mod ") != string::npos )
		{ 
			if ( buffer.find(" editor-> ") != string::npos )
			{	
				if ( buffer.find(" emacs") != string::npos ){ defaultEditor = "emacs " + path; };
				if ( buffer.find(" nvim") != string::npos ){ defaultEditor = "nvim " + path; };
				if ( buffer.find(" vim") != string::npos ){ defaultEditor = "vim " + path; };
				if ( buffer.find(" vi") != string::npos ){ defaultEditor = "vi " + path; };
			};
			if( buffer.find( " compiler-> " ) != string::npos )
			{
				if ( buffer.find(" g++") != string::npos ){ compiler = "g++"; };
				if ( buffer.find(" clang++") != string::npos ){ compiler = "clang++"; };
			};
			if ( buffer.find(" debug-> ") != string::npos )
			{ TEST
				if ( buffer.find(" true") != string::npos ){ results += "-g -O3 "; mkfileExtras = "	gdb --se=/tmp/a.out --readnow -q\0"; };
			};
		};

		if ( buffer.find("#include") != string::npos )
		{
			if ( buffer.find("Xlib.h") != string::npos ){ results += "-lX11 "; };
			if ( buffer.find("curl.h") != string::npos ){ results += "-lcurl "; };
			if ( buffer.find("SDL.h") != string::npos ){ results += "-lSDL2 "; };
			if ( buffer.find("SDL_image.h") != string::npos ){ results += "-lSDL2_image "; };
			if ( buffer.find("Imlib2.h") != string::npos ){ results += "-lImlib2 "; };
			if ( buffer.find("SDL_mixer.h") != string::npos){ results += "-lSDL2_mixer "; };
			if ( buffer.find("SDL2_gfxPrimitives.h") != string::npos ){ results += "-lSDL2_gfx "; };
			if ( buffer.find("SDL_ttf.h") != string::npos  ){ results += "-lSDL2_ttf "; };
		};

	};

	return results;

};

void makeFile(string flags){

	ofstream mkfile("/tmp/Makefile");

	mkfile << "a.out: test.cpp\n";
	mkfile << "	if [ -f a.out ]; then rm a.out; fi;\n";
	mkfile << "	";//tab required!!
	mkfile << compiler;
	mkfile << " test.cpp ";
	if ( flags != "" ){ mkfile << flags; };
	if ( mkfileExtras != "" )
	{
		mkfile << "\n";
		mkfile << mkfileExtras;
	};
	mkfile.close();

};

void help()
{

}

void createExampleFile(string wich)
{
	ofstream file("/tmp/test.cpp");

	if( wich.find("curl") != string::npos ){ file << curlExample; file.close(); return; };
	if( wich.find("sdl2_image") != string::npos ){ file << sdl2_ImageExample; file.close(); return; };
	if( wich.find("imlib2") != string::npos ){ file << imlib2Template; file.close(); return; };
	if( wich.find("xlib") != string::npos ){ file << xlibTemplate; file.close(); return; };

	cout << wich << " is not a valid argument for --template" << endl;
};

int main(int argc, char const *argv[])
{

	if ( argc > 1 )
	{
		bool templates = false;
		bool editor = false;
		for ( int i = 0; i < argc; i++ )
		{
			string aux = argv[i];

			if ( templates )
			{
				createExampleFile(aux);
				templates = false;
			};

			if ( editor )
			{
				defaultEditor = aux + " " + path;
				editor = false;
			};

			if ( aux.find("--help") != string::npos ){ cout << helpMessage << endl; return 0; };
			if ( aux.find("--template") != string::npos ){ templates = true; };
			if ( aux.find("--editor") != string::npos ){ editor = true; };
			if ( aux.find("--new") != string::npos ){ createNecesaryFiles("test.cpp"); };

		};
		if (templates){ cout << "--template require an argument!"; return 1; };
		if (editor) { cout << "--editor require an argument!"; return 1; };
	};

	string files = locateFile("/tmp");

	if ( files.find(".cpp") == std::string::npos )
	{
		createNecesaryFiles("test.cpp");
	}
	else
	{
		string trash = readFile();
	};

	system( defaultEditor.c_str() );

	string flags = readFile();
	
	cout << "flags: " << flags << endl;
	cout << "CC: " << compiler << endl;
	
	makeFile(flags);
	
	system("make -s -C /tmp");
	system("/tmp/a.out");
	
	return 0;
}
