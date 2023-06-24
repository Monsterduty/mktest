#CC = clang++ -std=c++17 -w -O3 -static #static release build
#CC = g++ -std=c++17 -w -O3 # release build
CC = clang++ -std=c++17 -O0 -g3 # debug build

OBJ_PATH = obj/
SRC_PATH = src/
INC_PATH = includes/
THIRD_PARTY_PATH = third-party/

PRECOMPILED_HEADERS = ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}testScriptMethods.hpp.gch ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}configParser.hpp.gch ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}terminalFontStyles.hpp.gch ${OBJ_PATH}resources.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch ${OBJ_PATH}templates.hpp.gch ${OBJ_PATH}qt6Templates.hpp.gch ${OBJ_PATH}libsResolver.hpp.gch ${OBJ_PATH}mktestInfoOutput.hpp.gch ${OBJ_PATH}manageArguments.hpp.gch ${OBJ_PATH}fileOperations.hpp.gch ${OBJ_PATH}cacheOperations.hpp.gch

OBJ_FILES = ${OBJ_PATH}utils.o ${OBJ_PATH}testScriptMethods.o ${OBJ_PATH}configParser.o ${OBJ_PATH}mktest.o ${OBJ_PATH}libsResolver.o ${OBJ_PATH}makefileRule.o ${OBJ_PATH}mktestInfoOutput.o ${OBJ_PATH}manageArguments.o ${OBJ_PATH}fileOperations.o ${OBJ_PATH}cacheOperations.o

ARGS = -I${OBJ_PATH} -I${INC_PATH} -I${THIRD_PARTY_PATH}

mktest: $(PRECOMPILED_HEADERS) $(OBJ_FILES)

	$(CC) ${ARGS} -I${OBJ_PATH} -I${INC_PATH} $(OBJ_FILES) -o mktest

${OBJ_PATH}mktest.o : ${SRC_PATH}mktest.cpp ${OBJ_PATH}terminalFontStyles.hpp.gch ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}mktestInfoOutput.hpp.gch 

	$(CC) ${ARGS} -c ${SRC_PATH}mktest.cpp -o ${OBJ_PATH}mktest.o

${OBJ_PATH}defaultIncludes.hpp.gch: ${INC_PATH}defaultIncludes.hpp

	$(CC) ${ARGS} ${INC_PATH}defaultIncludes.hpp -o ${OBJ_PATH}defaultIncludes.hpp.gch

${OBJ_PATH}templates.hpp.gch: ${INC_PATH}templates.hpp

	$(CC) ${ARGS} ${INC_PATH}templates.hpp -o ${OBJ_PATH}templates.hpp.gch

${OBJ_PATH}resources.hpp.gch: ${INC_PATH}resources.hpp

	$(CC) ${ARGS} ${INC_PATH}resources.hpp -o ${OBJ_PATH}resources.hpp.gch

${OBJ_PATH}terminalFontStyles.hpp.gch : ${THIRD_PARTY_PATH}terminalFontStyles.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${THIRD_PARTY_PATH}terminalFontStyles.hpp -o ${OBJ_PATH}terminalFontStyles.hpp.gch

${OBJ_PATH}makefileRule.hpp.gch: ${INC_PATH}makefileRule.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}makefileRule.hpp -o ${OBJ_PATH}makefileRule.hpp.gch

${OBJ_PATH}makefileRule.o: ${SRC_PATH}makefileRule.cpp ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}global.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}makefileRule.cpp -o ${OBJ_PATH}makefileRule.o

${OBJ_PATH}qt6Templates.hpp.gch : ${INC_PATH}qt6Templates.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}qt6Templates.hpp -o ${OBJ_PATH}qt6Templates.hpp.gch

${OBJ_PATH}configParser.hpp.gch : ${INC_PATH}configParser.hpp ${OBJ_PATH}defaultIncludes.hpp.gch
	
	$(CC) ${ARGS} ${INC_PATH}configParser.hpp -o ${OBJ_PATH}configParser.hpp.gch

${OBJ_PATH}configParser.o : ${SRC_PATH}configParser.cpp ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}utils.o ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}testScriptMethods.hpp.gch ${OBJ_PATH}testScriptMethods.o ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}configParser.cpp -o ${OBJ_PATH}configParser.o

${OBJ_PATH}global.hpp.gch : ${INC_PATH}global.hpp ${OBJ_PATH}makefileRule.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}global.hpp -o ${OBJ_PATH}global.hpp.gch

${OBJ_PATH}testScriptMethods.hpp.gch : ${INC_PATH}testScriptMethods.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}testScriptMethods.hpp -o ${OBJ_PATH}testScriptMethods.hpp.gch

${OBJ_PATH}testScriptMethods.o : ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}utils.o ${OBJ_PATH}testScriptMethods.hpp.gch ${SRC_PATH}testScriptMethods.cpp ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}testScriptMethods.cpp -o ${OBJ_PATH}testScriptMethods.o

${OBJ_PATH}utils.hpp.gch : ${INC_PATH}utils.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}utils.hpp -o ${OBJ_PATH}utils.hpp.gch

${OBJ_PATH}utils.o : ${SRC_PATH}utils.cpp ${OBJ_PATH}utils.hpp.gch

	$(CC) ${ARGS} -c ${SRC_PATH}utils.cpp -o ${OBJ_PATH}utils.o

${OBJ_PATH}libsResolver.hpp.gch: ${INC_PATH}libsResolver.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	$(CC) ${ARGS} ${INC_PATH}libsResolver.hpp -o ${OBJ_PATH}libsResolver.hpp.gch

${OBJ_PATH}libsResolver.o: ${SRC_PATH}libsResolver.cpp ${OBJ_PATH}libsResolver.hpp.gch ${OBJ_PATH}qt6Templates.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}libsResolver.cpp -o ${OBJ_PATH}libsResolver.o

${OBJ_PATH}mktestInfoOutput.hpp.gch: ${INC_PATH}mktestInfoOutput.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	${CC} ${ARGS} ${INC_PATH}mktestInfoOutput.hpp -o ${OBJ_PATH}mktestInfoOutput.hpp.gch

${OBJ_PATH}mktestInfoOutput.o : ${SRC_PATH}mktestInfoOutput.cpp ${OBJ_PATH}mktestInfoOutput.hpp.gch ${OBJ_PATH}defaultIncludes.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}mktestInfoOutput.cpp -o ${OBJ_PATH}mktestInfoOutput.o

${OBJ_PATH}manageArguments.hpp.gch : ${INC_PATH}manageArguments.hpp

	${CC} ${ARGS} ${INC_PATH}manageArguments.hpp -o ${OBJ_PATH}manageArguments.hpp.gch

${OBJ_PATH}manageArguments.o : ${SRC_PATH}manageArguments.cpp ${OBJ_PATH}manageArguments.hpp.gch ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}mktestInfoOutput.hpp.gch ${OBJ_PATH}configParser.hpp.gch ${OBJ_PATH}fileOperations.hpp.gch ${OBJ_PATH}cacheOperations.hpp.gch ${OBJ_PATH}terminalFontStyles.hpp.gch ${OBJ_PATH}testScriptMethods.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}manageArguments.cpp -o ${OBJ_PATH}manageArguments.o

${OBJ_PATH}fileOperations.hpp.gch : ${INC_PATH}fileOperations.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	${CC} ${ARGS} ${INC_PATH}fileOperations.hpp -o ${OBJ_PATH}fileOperations.hpp.gch

${OBJ_PATH}fileOperations.o : ${SRC_PATH}fileOperations.cpp ${OBJ_PATH}fileOperations.hpp.gch ${OBJ_PATH}global.hpp.gch ${OBJ_PATH}mktestInfoOutput.hpp.gch ${OBJ_PATH}utils.hpp.gch ${OBJ_PATH}cacheOperations.hpp.gch ${OBJ_PATH}libsResolver.hpp.gch ${OBJ_PATH}templates.hpp.gch ${OBJ_PATH}qt6Templates.hpp.gch ${OBJ_PATH}resources.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}fileOperations.cpp -o ${OBJ_PATH}fileOperations.o

${OBJ_PATH}cacheOperations.hpp.gch : ${INC_PATH}cacheOperations.hpp ${OBJ_PATH}defaultIncludes.hpp.gch

	${CC} ${ARGS} ${INC_PATH}cacheOperations.hpp -o ${OBJ_PATH}cacheOperations.hpp.gch 

${OBJ_PATH}cacheOperations.o : ${SRC_PATH}cacheOperations.cpp ${OBJ_PATH}cacheOperations.hpp.gch ${OBJ_PATH}global.hpp.gch

	${CC} ${ARGS} -c ${SRC_PATH}cacheOperations.cpp -o ${OBJ_PATH}cacheOperations.o

clean:

	rm -f mktest mktest* ${OBJ_PATH}*.gch ${OBJ_PATH}*.o
