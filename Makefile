.PHONY: clean all

all: example

example.o: example.cpp async_ostream.hpp concurrent.hpp concurrent_queue.hpp
	g++ -std=gnu++11 -o example.o -c example.cpp

example: example.o
	g++ -std=gnu++11 -o example example.o -pthread -lboost_thread -lboost_system -Wall
	
clean:
	-rm example *.o 

