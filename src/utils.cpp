#include "utils.hpp"
#include "global.hpp"

bool utils::FileExist( std::string name )
{
	if ( name == "" )
		return false;

	if ( name.find('/') == std::string::npos )
		name = "/" + name;

	std::filesystem::path file( path + name );

	if ( std::filesystem::exists( file ) )
		return true;
	return false;
}

int utils::compString( std::string &s1, std::string &s2 )
{
	if ( s1.size() == 0 || s2.size() == 0 )
		return 0;

	for ( uint64_t i = 0; i < std::max( s1.size(), s2.size() ); i++ )
	{
		if ( i > std::min( s1.size(), s2.size() ) )
			return i -1;

		if ( s1[i] != s2[i] )
			return i;
	}

	return -1;
}

//#include <iostream>
void utils::keepStringStructure( std::string required, std::string base, std::string &source )
{
	if ( !required.size() || !base.size() || !source.size() )
		return;


	size_t pos = source.find(base);
	while( pos != std::string::npos )
	{
	//	std::cout << "called keepStringStructure" << std::endl;
		source = source.substr(0, pos) + required + source.substr( pos+base.size(), source.size() );
		pos = source.find( base, pos+ required.size() );
	}
}

void utils::replaceByString( std::string replace, std::string withThis, std::string &source )
{
	size_t pos = source.find( replace );
	while ( pos != std::string::npos )
	{
		source = source.substr( 0, pos ) + withThis + source.substr( pos + replace.size(), source.length() );
		pos = source.find( replace );
	}
}
