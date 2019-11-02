fix: tradeclient.o Application.o
	 g++ -o fix-client  tradeclient.o Application.o -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

application.o: Application.cpp Application.h
	 g++ -c Application.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz

tradeclient.o: tradeclient.cpp
	 g++ -c tradeclient.cpp -fexceptions -finline-functions -lquickfix -lpthread -lxml2 -lz
