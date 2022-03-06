CC=g++
CFLAGS=-Wall -g -std=c++11
all: popdb

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $^

popdb: executors.o expression.o utils.o db_info.o main.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *.o *.gch