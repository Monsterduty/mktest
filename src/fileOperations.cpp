#include "fileOperations.hpp"
#include "global.hpp"
#include "mktestInfoOutput.hpp"
#include "utils.hpp"
#include "cacheOperations.hpp"
#include "libsResolver.hpp"
#include "templates.hpp"
#include "qt6Templates.hpp"
#include "resources.hpp"

std::vector<std::string> getListFiles( std::string mainFilePath )
{
	std::ifstream file( mainFilePath );
	std::vector<std::string> ret;

	while ( file.good() )
	{
		std::string element = "";
        	file >> element;

        	if ( element != "#include" )
        		continue;

        	file >> element;
    		if ( element.find('"') != std::string::npos && element.find(".h") != std::string::npos )
        	{
       			//clear quotation marks from the string to get the normal name
       			element = element.substr( 1, element.length() -2 );
           	
        		if ( utils::FileExist(element) )
    			{	
            			//if the element file anm isn't in the file vector, add it,
        			if ( std::count( ret.begin(), ret.end(), element ) < 1 )
    					ret.push_back(element);

            			//adding linked files.
            		
				std::vector aux = getListFiles( path + "/" + element);
            			//cout << "vectorAux: " << aux.back() << endl;
            			for ( auto files : aux )
            				if ( std::count( ret.begin(), ret.end(), files ) < 1 )
            					ret.push_back(files);
        		}

        		//checking if there's a source file
        		element = element.substr( 0, element.find(".h") ) + ".cpp";
                
        		if ( utils::FileExist(element) )
        		{
               			//adding file if not exist. 
               			if ( std::count( ret.begin(), ret.end(), element ) < 1 )
               				ret.push_back(element);
                	
               			//reading for files linked.
				std::vector aux = getListFiles( path + "/" + element );
               			for( auto files : aux )
               				if ( std::count( ret.begin(), ret.end(), files ) < 1 )
               					ret.push_back(files);
        		}
		}
	}

	return ret;
}

//file creation handler
void createNecesaryFiles( std::string fileName)
{
	//scan the path of your file for a previous file with the same name of the new
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		if ( dir.path().string().find(fileName) != std::string::npos )
		{
			std::string aux = "";

			//report the problem to the user and ask for what should do.
			throwMessage( " [ " + fileName + " ] this file already exist.", THROW_CODE::WARNING );
			std::cout << "What should we do? [replace/edit/none]: ";
			std::cin >> aux;
			if ( aux.find('r') != std::string::npos )
			{
				std::cout << "replacing" << std::endl;
				break;
			}
			else if ( aux.find('e') != std::string::npos )
			{
				file = fileName;
				return;
			}
			else
			{
				std::cout << "aborting" << std::endl;
				exit(0);
			}
		}

	//where create the file and stream to the file
	std::string name = path + '/' + fileName;
	std::ofstream saveFile (name);

	//if the new file is a *.cpp file create a c++ template
	if ( fileName.find(".cpp") != std::string::npos )
	{
		if ( modArgs != "//mod->" )
			saveFile << modArgs << std::endl;

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
	if ( fileName.find(".h") != std::string::npos )
	{
		std::string define = "";
		for ( int i = 0; i < fileName.length(); i++ )
			define.push_back( fileName[i] == '.' ? '_' : toupper( fileName[i] ) );
		saveFile << "#ifndef " << define << std::endl;
		saveFile << "#define " << define << std::endl << std::endl;
		saveFile << "//code here" << std::endl << std::endl;
		saveFile << "#endif";
	}
	else
		//if no is a kown type of file just print the name of the file inside that file
		saveFile << "//" + fileName;
	saveFile.close();

	//set the file as default for edit
	file = fileName;
};

//save your code file on the provided path
void saveCode( std::string onThisPlace )
{
	if ( onThisPlace == "" )
		onThisPlace = home + "/mktestProj";
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
	std::vector<std::string> filesToRead = getListFiles( path + file );
	filesToRead.push_back(file);
	filesToRead.push_back( "Makefile" );

	//knowing that the [ file ] variable has an slash, it was necesary to remove it from the [ onThisPlace ] variable if exist
	

	//saving any linked file of your project
	for ( auto files : filesToRead )
	{
		std::ofstream fileToSave( std::string( onThisPlace + files ) );
		std::ifstream codeFile( path + "/" + files );
	
		//the rdbuf() method allow to copy all the file content once,
		fileToSave << codeFile.rdbuf();

		fileToSave.close();
		codeFile.close();

	}

	//report path to the user
	throwMessage( "save on: " + onThisPlace, THROW_CODE::WARNING );

	exit(0);
}

//if the file exist delete the file
void deleteFile( std::string File)
{
	std::string search;

	//read the directory to get te files in there.
	for ( auto &entry : std::filesystem::directory_iterator( path ) )
		search += entry.path().string(), search += " ";

	//if the name of the file contain a slash, this will remove that to find the name on the search string
	std::string name = File.find('/') != std::string::npos ? File.substr( 1, File.length() ) : File;

	//if the file exist, delete it, if not, report to the user.
	if ( search.find( name ) != std::string::npos )
		std::filesystem::remove( path + File );
	else
		throwMessage( path + File + " doesn't exist yet", THROW_CODE::ERROR );
		exit(1);
}

//read your code file for comment flags and required argument to compile
std::vector<std::string> readFile( std::vector<std::string> *recursivePreviousFlags )
{
	//reset some variables and values that change again if their are nedded.
	if ( reEdit ) reEdit = false;
	writeCache("fileToEdit", "test.cpp");
	debug = "";

	//stream from your code file
	std::ifstream reading(path+file);

	//buffer to read the whole lines and a result for return the nedded flags for compile your code
	std::string buffer = "";
	std::vector<std::string> results = {};

	if ( recursivePreviousFlags )
		results = *recursivePreviousFlags;

	if ( customCompileArgs )
		results.push_back( customCompileArgsString );
	
	while ( getline( reading, buffer ) )
	{
		//detecting mod args
		if ( buffer.find("//mod-> ") != std::string::npos )
		{
			std::vector<std::string> fileModArgs = {};
			std::string word = "";
			for ( char &c : buffer )
				if ( c != ' ' && c != '\t' && &c != &buffer.back() )
					word.push_back(c);
				else
				{
					if ( word == "" )
						continue;
					if ( &c == &buffer.back() )
						word.push_back(c);
					fileModArgs.push_back(word);
					word.clear();
				}

			//the kind of editor if was specified, if isn't specified by default will use nano
			if ( utils::find( fileModArgs.begin(), fileModArgs.end(), "editor->" ) )
			{
				size_t pos = utils::whereIs( fileModArgs.begin(), fileModArgs.end(), "editor->" );
				editor = fileModArgs.size() >= pos ? fileModArgs.at(pos+1) + " " : editor;
			};

			//the compiler to use, if isn't specified by default will use g++
			if( utils::find( fileModArgs.begin(), fileModArgs.end(), "compiler->" ) )
			{
				size_t pos = utils::whereIs( fileModArgs.begin(), fileModArgs.end(), "compiler->" );
				compiler = fileModArgs.size() >= pos ? fileModArgs.at(pos+1) + " " : compiler;
			};

			 //coplile the code with debug flags and open the executable with gdb for debug
			if ( utils::find( fileModArgs.begin(), fileModArgs.end(), "debug" ) )
			{ 
				results.push_back( "-g -O3");
				debug = "\tgdb --se=" + path + executable + " --readnow -q\0";
			};

			if ( utils::find( fileModArgs.begin(), fileModArgs.end(),"keep") ) writeCache("fileToEdit: ", file);

			//active the [ reEdit ] variable for not get out from the mktest program
			if ( utils::find( fileModArgs.begin(), fileModArgs.end(),"reEdit") ) reEdit = true;

			//use language version
			if ( utils::find( fileModArgs.begin(), fileModArgs.end() ,"c++->") && !customCompileArgs )
			{
				size_t pos = utils::whereIs( fileModArgs.begin(), fileModArgs.end(), "c++->" );
				results.push_back( fileModArgs.size() >= pos ? "-std=c++" + fileModArgs.at(pos+1) : "" );
			}
		};

		//to give arguments for your executable, see mktest --help for more info
		if ( buffer.find( "//args-> " ) != std::string::npos )
		{
			programsArgs = "";
			for ( int i = 9; i < buffer.size(); i++ )
				programsArgs+= buffer[i];
		};

		//detecting libraries and headers,
		if ( !customCompileArgs )
		if ( buffer.find("#include") != std::string::npos )
		{
			resolveLibsFlags(buffer, results);

			//for MakeFile rules
			if ( buffer.find('"') != std::string::npos )
			{	
				//cleaning string for have only the file name
				buffer = buffer.substr( buffer.find('"') + 1, buffer.length() );
				buffer = buffer.substr(0, buffer.find('"') );

				if ( buffer.find(".h") != std::string::npos )
				{ 
					//This lamda function allows to read recursively all the file linked to our test.cpp file
					//and it's tree hierarchy.
					auto recursiveLecture = [&]( std::string &objective ) {
						for ( std::string &item : openedFiles )
							if ( path + "/" + objective == item )
								return;
						openedFiles.push_back( path + "/" + objective );
						std::string oldFile = file;
						file = "/" + objective;
						results = readFile( &results );
						file = oldFile;
					};

					if ( utils::FileExist( buffer ) )
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
					std::string source = buffer.substr(0, buffer.find(".h")) + ".cpp";
					 if ( utils::FileExist( source ) )
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

//this funcion create a template of a predefined type of project for some libraries
void createExampleFile( std::string wich )
{
	//if the working directory doesn't exist, create one.
	if ( !std::filesystem::exists(path) )
		std::filesystem::create_directory(path);

	//delete the previous content of the directory
	for ( auto &entry : std::filesystem::directory_iterator(path) )
		std::filesystem::remove_all(entry.path());

	//stream for the current code file
	std::ofstream codeFile( path + file, std::ios::out | std::ios::binary );

	//templates availables
	if( wich == "curl" ){ codeFile << curlExample; codeFile.close(); return; };
	if( wich == "imlib2" ){ codeFile << imlib2Template; codeFile.close(); return; };
	if( wich == "xlib" ){ codeFile << xlibTemplate; codeFile.close(); return; };
	if ( wich == "sdl2" ){ codeFile << sdl2_example; codeFile.close(); return; }
	if( wich == "sdl2_image" )
	{
		std::ofstream imageData ( path +"/image.jpg", std::ios::out | std::ios::binary);
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
		std::ofstream file( path + "/window.hpp", std::ios::out | std::ios::binary );
		file << Qt6::window_hpp;
		file.close();
		file = std::ofstream( path + "/window.cpp", std::ios::out | std::ios::binary );
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


