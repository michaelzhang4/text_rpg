
TARGET = main.exe

OBJECTS = main.o helpers.o
all: compile
	$(TARGET)

compile: helpers.o main.o
	g++ -o $(TARGET) $(OBJECTS) 

helpers.o: helpers.cpp helpers.hpp
	g++ -c helpers.cpp

main.o: main.cpp helpers.hpp
	g++ -c main.cpp