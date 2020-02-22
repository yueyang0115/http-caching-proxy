all: main

main: main.cpp function.cpp proxy.cpp proxy.h parse.cpp
	g++ -g  -o main main.cpp function.cpp proxy.cpp parse.cpp -lpthread

.PHONY:
	clean
clean:
	rm -rf *.o main
