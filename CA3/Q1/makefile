CC=g++

CFLAGS=-c

INCDIR=`pkg-config opencv4 --cflags`


LIB = `pkg-config opencv4  --libs`

CST11=-std=c++11

all: main

main: main.o 
	$(CC) $(LIB) $(CST11) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(CST11) main.cpp

clean:
	rm -f *o main

