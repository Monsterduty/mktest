#pragma once

#include "defaultIncludes.hpp"

void readCache();

//this generate/write a cache file to remeber changes tempolary
void writeCache( std::string whichTag, std::string value);
