all: main

main: main.cpp function.cpp proxy.cpp proxy.h parse.cpp response.cpp
	g++ -g  -o main main.cpp function.cpp proxy.cpp parse.cpp response.cpp -lpthread

.PHONY:
	clean
clean:
	rm -rf *.o main
