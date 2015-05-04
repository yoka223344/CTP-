ExportMarket : demo.o MdSpi.o Logger.o TimeConvert.o ClientBase.o Client.o DataFetch.o
	g++ -o ExportMarket -g demo.o Logger.o TimeConvert.o \
		ClientBase.o DataFetch.o MdSpi.o Client.o\
		-L./ThostTraderApi -lthostmduserapi\
		-lthosttraderapi -lmysqlclient -llog4cpp -lpthread -lz
demo.o : demo.cpp 
	g++ -c -g demo.cpp 
MdSpi.o : MdSpi.cpp 
	g++ -c -g MdSpi.cpp
Logger.o : Logger.cpp
	g++ -c -g Logger.cpp
TimeConvert.o : TimeConvert.cpp
	g++ -c -g TimeConvert.cpp
ClientBase.o : ClientBase.cpp
	g++ -c -g ClientBase.cpp
Client.o : Client.cpp
	g++ -c -g Client.cpp
DataFetch.o : DataFetch.cpp
	g++ -c -g DataFetch.cpp
clean :
	rm ExportMarket *.o
