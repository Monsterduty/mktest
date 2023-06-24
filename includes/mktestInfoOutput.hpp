#pragma once
#include "defaultIncludes.hpp"

void help(); 

enum THROW_CODE
{
	ERROR,
	WARNING
};

void throwMessage( std::string message, THROW_CODE messageType );

void throwError( std::string wrongArg, std::string flag, std::string whitchError );
