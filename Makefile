#CC = clang++ -std=c++17 -w -O3 -static #static release build
CC = clang++ -std=c++17 -w -O3 # release build
#CC = clang++ -std=c++17 -O0 -g3 # debug build

PRECOMPILED_HEADERS = obj/utils.hpp.gch obj/testScriptMethods.hpp.gch obj/global.hpp.gch obj/configParser.hpp.gch obj/makefileRule.hpp.gch obj/terminalFontStyles.hpp.gch obj/resources.hpp.gch obj/defaultIncludes.hpp.gch obj/templates.hpp.gch obj/qt6Templates.hpp.gch

OBJ_FILES = obj/utils.o obj/testScriptMethods.o obj/configParser.o obj/mktest.o

OBJ_PATH = obj/
SRC_PATH = src/
INC_PATH = includes/
THIRD_PARTY_PATH = third-party/

ARGS = -I${OBJ_PATH} -I${INC_PATH} -I${THIRD_PARTY_PATH}

mktest: $(PRECOMPILED_HEADERS) $(OBJ_FILES)

	$(CC) ${ARGS} -I${OBJ_PATH} -I${INC_PATH} $(OBJ_FILES) -o mktest

obj/mktest.o : ${SRC_PATH}mktest.cpp ${OBJ_PATH}templates.hpp.gch ${OBJ_PATH}qt6Templates.hpp.gch ${OBJ_PATH}resources.hpp.gch ${OBJ_PATH}terminalFontStyles.hpp.gch ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}configParser.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}mktest.cpp -o ${OBJ_PATH}mktest.o

obj/defaultIncludes.hpp.gch: ${INC_PATH}defaultIncludes.hpp

	$(CC) ${ARGS} ${INC_PATH}defaultIncludes.hpp -o ${OBJ_PATH}defaultIncludes.hpp.gch

obj/templates.hpp.gch: ${INC_PATH}templates.hpp

	$(CC) ${ARGS} ${INC_PATH}templates.hpp -o ${OBJ_PATH}templates.hpp.gch

obj/resources.hpp.gch: ${INC_PATH}resources.hpp

	$(CC) ${ARGS} ${INC_PATH}resources.hpp -o ${OBJ_PATH}resources.hpp.gch

obj/terminalFontStyles.hpp.gch : ${THIRD_PARTY_PATH}terminalFontStyles.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${THIRD_PARTY_PATH}terminalFontStyles.hpp -o ${OBJ_PATH}terminalFontStyles.hpp.gch

obj/makefileRule.hpp.gch: ${INC_PATH}makefileRule.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}makefileRule.hpp -o ${OBJ_PATH}makefileRule.hpp.gch

obj/qt6Templates.hpp.gch : ${INC_PATH}qt6Templates.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}qt6Templates.hpp -o ${OBJ_PATH}qt6Templates.hpp.gch

obj/configParser.hpp.gch : ${INC_PATH}configParser.hpp ${OBJ_PATH}defaultIncludes.hpp.gch
	
	$(CC) ${ARGS} ${INC_PATH}configParser.hpp -o ${OBJ_PATH}configParser.hpp.gch

obj/configParser.o : ${SRC_PATH}configParser.cpp ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}utils.o ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}testScriptMethods.hpp.gch ${OBJ_PATH}testScriptMethods.o ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}configParser.cpp -o ${OBJ_PATH}configParser.o

obj/global.hpp.gch : ${INC_PATH}global.hpp ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}global.hpp -o ${OBJ_PATH}global.hpp.gch

obj/testScriptMethods.hpp.gch : ${INC_PATH}testScriptMethods.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}testScriptMethods.hpp -o ${OBJ_PATH}testScriptMethods.hpp.gch

obj/testScriptMethods.o : ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}utils.o ${OBJ_PATH}programEnvironmentStruct.hpp.gch ${OBJ_PATH}testScriptMethods.hpp.gch ${SRC_PATH}testScriptMethods.cpp ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}testScriptMethods.cpp -o ${OBJ_PATH}testScriptMethods.o

obj/programEnvironmentStruct.hpp.gch : ${INC_PATH}programEnvironmentStruct.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}programEnvironmentStruct.hpp -o ${OBJ_PATH}programEnvironmentStruct.hpp.gch

obj/utils.hpp.gch : ${INC_PATH}utils.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}utils.hpp -o ${OBJ_PATH}utils.hpp.gch

obj/utils.o : ${SRC_PATH}utils.cpp ${OBJ_PATH}utils.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}utils.cpp -o ${OBJ_PATH}utils.o

clean:

	rm -f mktest obj/*.gch obj/*.o
