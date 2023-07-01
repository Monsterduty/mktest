#ifndef UTILS_HPP
#define UTILS_HPP
#include "defaultIncludes.hpp"

namespace utils
{
	template<class InputIterator, typename T>
	int whereIs( InputIterator begin, InputIterator end, T match )
	{
		int pos = 0;
		while ( begin != end )
			if ( *begin == match )
				return pos;
			else
			{
				pos++;
				begin++;
			}
		return -1;
	}
	bool FileExist( std::string name );
	//return -1 if the strings are equals, otherwise return the place where the string is diferent from s1 to s2. 
	int compString( std::string &s1, std::string &s2 );
	void replaceByString( std::string replace, std::string withThis, std::string &source );
	void keepStringStructure( std::string required, std::string base, std::string &source ); 
	template <class InputIterator, typename T>
		bool find( InputIterator begin, InputIterator end, T match )
		{
			while( begin != end )
			{
				if ( *begin == match )
					return true;
				begin++;
			}
			return false;
		}
}

#endif
