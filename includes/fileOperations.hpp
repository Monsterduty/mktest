#ifndef FILE_OPERATIONS_HPP
#define FILE_OPERATIONS_HPP

#include "defaultIncludes.hpp"

std::vector<std::string> readFile( std::vector<std::string> *recursivePreviousFlags = nullptr );

void createNecesaryFiles( std::string fileName );

//this funcion create a template of a predefined type of project for some libraries
void createExampleFile( std::string wich );

//if the file exist delete the file
void deleteFile( std::string File);

//save your code file on the provided path
void saveCode( std::string onThisPlace = "" );

#endif
