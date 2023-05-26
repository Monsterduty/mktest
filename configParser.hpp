#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

struct programEnvironmentStruct;
#include <string>

void readConfig( std::string file );

bool setEnvironment( std::string name );

#endif
