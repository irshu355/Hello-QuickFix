fix: executor.o application.o
	 g++ -o fix-executor  executor.o application.o -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

application.o: application.cpp application.h
	 g++ -c application.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

executor.o: executor.cpp
	 g++ -c executor.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz