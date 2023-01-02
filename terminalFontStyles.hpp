#include <string>
#include <iostream>

#define TEXT_RESET "\033[0m"

//stiles

#define TEXT_BOLD "\033[1m"
#define TEXT_UNDERLINE "\033[4m"

//colors
#define TEXT_BLACK "\033[30m"
#define TEXT_RED "\033[31m"
#define TEXT_GREEN "\033[32m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_BLUE "\033[34m"
#define TEXT_MAGENTA "\033[35m"
#define TEXT_CYAN "\033[36m"
#define TEXT_WHITE "\033[37m"
#define TEXT_BRIGHT_BLACK "\033[90m"
#define TEXT_BRIGHT_RED "\033[91m"
#define TEXT_BRIGHT_GREEN "\033[92m"
#define TEXT_BRIGHT_YELLOW "\033[93m"
#define TEXT_BRIGHT_BLUE "\033[94m"
#define TEXT_BRIGHT_MAGENTA "\033[95m"
#define TEXT_BRIGHT_CYAN "\033[96m"
#define TEXT_BRIGHT_WHITE "\033[97m"

#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\33[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

std::string TEXT_RGB( int R = 0, int G = 0, int B = 0 )
{
	return "\033[38;2;" + std::to_string(R) + ";" + std::to_string(G) + ";" + std::to_string(B) + "m";
}

std::string BG_RGB( int R = 0, int G = 0, int B = 0 )
{
	return "\033[48;2;" + std::to_string(R) + ";" + std::to_string(G) + ";" + std::to_string(B) + "m";
}