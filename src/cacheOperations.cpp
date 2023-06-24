
#include "global.hpp"
#include "cacheOperations.hpp"

//read the current cache file
void readCache()
{
	//stream from cache file
	std::ifstream cacheFile( path + cacheFileName );

	//read the chache file
	while( cacheFile.good() )
	{
		std::string tag;
		cacheFile >> tag;

		//aply any change of the cache file to the mktest program
		if ( tag == "fileToEdit:" )
			file.clear(),
			cacheFile >> file;
	}
}

//this generate/write a cache file to remeber changes tempolary
void writeCache( std::string whichTag, std::string value)
{
	//string where the content of the directory will be saved.
	std::string search = "";

	//string where the cache info will be saved before be writen in the cache file
	std::string info = "";

	//read directory to find what files are there.
	for ( auto dir : std::filesystem::directory_iterator( path ) )
		search += dir.path().string() + " ";

	//check if there's a previous cache file.
	if ( search.find( cacheFileName.substr(1, cacheFileName.length()) ) != std::string::npos )
	{
		//stream from the previous cache file
		std::ifstream reading( path + cacheFileName );

		//pass the information of that cache file to the [ info ] string
		//and make the required changes acording to the function argumets
		while ( reading.good() )
		{
			//passing the information of the stream to a string word by word
			std::string tag;
			reading >> tag;

			//if the previous cache file has a tag equal to what you want
			//to save/edit, remplace that with the new value
			if ( tag == whichTag )
			{
				info += whichTag + " ";
				info += ( value.find('/') != std::string::npos ? value.substr(1, value.length()) : value );
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
				info += ( value.find('/') != std::string::npos ? value.substr(1, value.length()) : value );
			}
			else
			{
				info += '\n' + whichTag + " ";
				info += ( value.find('/') != std::string::npos ? value.substr(1, value.length()) : value );
			}
		}

		//close the data stream
		reading.close();
	}
	else
		//if a previus cache file doesn't exist, just write the desired values on a new file
		info = whichTag + " " + value;

	//create a new cache file with the desired information.
	std::ofstream writing( path + cacheFileName );

	//write the data.
	writing << info;

	//close the stream.
	writing.close();
}
