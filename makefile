CC=g++
CFLAGS=-Wall -g -std=c++17
all: popdb

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $^

popdb: expression.o utils.o base_reader.o base_controller.o base_common.o base.o record_constructor.o main.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *.o *.gch