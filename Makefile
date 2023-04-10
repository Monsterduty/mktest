CC = g++ -std=c++17 -w -O2

mktest: mktest.cpp makefileRule.hpp.gch terminalFontStyles.hpp.gch resources.hpp.gch defaultIncludes.hpp.gch templates.hpp.gch qt6Templates.hpp.gch

	$(CC) mktest.cpp -o mktest

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

clean:

	rm -f mktest *gch
