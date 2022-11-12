CC = g++

mktest: mktest.cpp defaultIncludes.hpp.gch templates.hpp.gch

	$(CC) mktest.cpp -o mktest

defaultIncludes.hpp.gch: defaultIncludes.hpp

	$(CC) defaultIncludes.hpp

templates.hpp.gch: templates.hpp

	$(CC) templates.hpp

clean:

	rm -f mktest *gch