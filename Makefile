CC = g++


mktest: mktest.cpp defaultIncludes.h.gch templates.h.gch

	$(CC) mktest.cpp -o mktest

defaultIncludes.h.gch: defaultIncludes.h

	$(CC) defaultIncludes.h.gch

templates.h.gch: templates.h

	$(CC) templates.h