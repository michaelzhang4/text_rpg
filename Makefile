
TARGET = main.exe

OBJECTS = main.o helpers.o
all: compile
	$(TARGET)

compile: helpers.o main.o
	g++ -g -o $(TARGET) $(OBJECTS) 

helpers.o: helpers.cpp helpers.hpp
	g++ -g -c helpers.cpp

main.o: main.cpp helpers.hpp
	g++ -g -c main.cpp