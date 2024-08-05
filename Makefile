TARGET = main.exe

FLAGS = g++

OBJECTS = main.o helpers.o

all: compile
	$(TARGET)

compile: helpers.o main.o
	$(FLAGS) -o $(TARGET) $(OBJECTS) 

helpers.o: helpers.cpp helpers.hpp
	$(FLAGS) -c helpers.cpp

main.o: main.cpp helpers.hpp
	$(FLAGS) -c main.cpp