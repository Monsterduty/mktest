
//#include <string>
//#include <vector>
#include "defaultIncludes.hpp"
#include "makefileRule.hpp"

#if defined(__linux__) && defined(__ANDROID__)
auto getPath = [](){ 
	std::string ret = getenv("TMPDIR");
	if ( ret.empty() )
	{
		ret = getenv("HOME");
		ret = ret.substr(0, ret.find_last_of("/")) + "/usr/tmp";
	}
	return ret;
};
inline std::string path = getPath();
inline std::string home = getenv("HOME");
inline std::string executable = "/a.out";
inline std::string configFile = home + "/.config/mktest/config.conf";
#else
inline std::string path = "/tmp/mktestDir";
inline std::string home = getenv("HOME");
inline std::string executable = "/a.out";
inline std::string configFile = home + "/.config/mktest/config.conf";
#endif

#ifdef __WIN32
inline std::string path = getenv("TMP") + "/mktestDir";
inline std::string home = getenv("USERPROFILE");
inline std::string executable = "/a.exe";
inline std::string configFile = getenv("appdata") + "/local/mktest/config.conf";
#endif

inline std::string file = "/test.cpp";

inline std::string editor = "nano ";
inline std::string compiler = "g++";
inline std::string debug = "";
inline std::string defaultEditor = "";
inline std::string programsArgs = "";
inline std::string modArgs = "//mod->";
inline std::string cacheFileName = "/mktest.cache";
inline bool reEdit = false;
inline std::vector<makefileRule> mkfileRules;

struct programEnvironmentStruct;
inline std::vector<programEnvironmentStruct*> programEnvironments = {};
