service : threadpool.o ServiceBase.o service.o strategy.o client.o
	g++ -o service service.o threadpool.o ServiceBase.o  -lpthread -lz
	g++ -o client client.o strategy.o
threadpool.o : threadpool.cpp 
	g++ -c threadpool.cpp
ServiceBase.o : ServiceBase.cpp
	g++ -c ServiceBase.cpp
service.o : service.cpp
	g++ -c service.cpp
strategy.o : strategy.cpp
	g++ -c strategy.cpp
client.o : client.cpp
	g++ -c client.cpp
clean :
	rm service client *.o
