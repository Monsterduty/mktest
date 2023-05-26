
#include <string>
#include <vector>
#include "makefileRule.hpp"

#ifdef __linux__
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
