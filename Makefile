#CC = clang++ -std=c++17 -w -O3 # release build
CC = clang++ -std=c++17 -O0 -g3 # debug build

PRECOMPILED_HEADERS = utils.hpp.gch testScriptMethods.hpp.gch global.hpp.gch configParser.hpp.gch makefileRule.hpp.gch terminalFontStyles.hpp.gch resources.hpp.gch defaultIncludes.hpp.gch templates.hpp.gch qt6Templates.hpp.gch

OBJ_FILES = utils.o testScriptMethods.o configParser.o mktest.o

mktest: mktest.o $(PRECOMPILED_HEADERS) $(OBJ_FILES)

	$(CC) $(OBJ_FILES) -o mktest

mktest.o : mktest.cpp

	$(CC) -c mktest.cpp

defaultIncludes.hpp.gch: defaultIncludes.hpp

	$(CC) defaultIncludes.hpp

templates.hpp.gch: templates.hpp

	$(CC) templates.hpp

resources.hpp.gch: resources.hpp

	$(CC) resources.hpp

terminalFontStyles.hpp.gch : terminalFontStyles.hpp

	$(CC) terminalFontStyles.hpp

makefileRule.hpp.gch: makefileRule.hpp

	$(CC) makefileRule.hpp

qt6Templates.hpp.gch : qt6Templates.hpp

	$(CC) qt6Templates.hpp

configParser.hpp.gch : configParser.hpp

	$(CC) configParser.hpp

configParser.o : utils.hpp.gch utils.o configParser.cpp

	$(CC) -c configParser.cpp

global.hpp.gch : global.hpp makefileRule.hpp.gch

	$(CC) global.hpp

testScriptMethods.hpp.gch : testScriptMethods.hpp

	$(CC) testScriptMethods.hpp

testScriptMethods.o : utils.hpp.gch utils.o programEnvironmentStruct.hpp.gch testScriptMethods.hpp.gch testScriptMethods.cpp global.hpp.gch

	$(CC) -c testScriptMethods.cpp

programEnvironmentStruct.hpp.gch : programEnvironmentStruct.hpp

	$(CC) programEnvironmentStruct.hpp

utils.hpp.gch : utils.hpp

	$(CC) utils.hpp

utils.o : utils.cpp utils.hpp.gch

	$(CC) -c utils.cpp

clean:

	rm -f mktest *.gch *.o
