
//#include <string>
//#include <vector>
#include "defaultIncludes.hpp"
#include "makefileRule.hpp"

#if defined(__linux__)

	inline std::string path = "/tmp/mktestDir";
	inline std::string home = getenv("HOME");
	inline std::string executable = "/a.out";
	inline std::string configFile = home + "/.config/mktest/config.conf";
	inline std::string version = "mktest 2.0 linux";

#elif defined(__linux__) && defined(__ANDROID__)
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
	inline std::string version = "mktest 2.0 android";

#elif defined(__WIN32)
	#include <processenv.h>

	inline std::string path = std::string(getenv("TMP")) + "/mktestDir";
	inline std::string home = std::string(getenv("USERPROFILE"));
	inline std::string executable = "/a.exe";
	inline std::string configFile = std::string(getenv("appdata")) + "/mktest/config.conf";
	inline std::string version = "mktest 2.0 windows";

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

