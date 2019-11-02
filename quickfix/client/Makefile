fix: tradeclient.o application.o
	 g++ -o fix-client  tradeclient.o application.o -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

application.o: application.cpp application.h
	 g++ -c application.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

tradeclient.o: tradeclient.cpp
	 g++ -c tradeclient.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz