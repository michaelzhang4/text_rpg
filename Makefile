TARGET = textrpg.exe

FLAGS = g++

OBJECTS = main.o helpers.o content.o classes.o

all: compile
	$(TARGET)

compile: $(OBJECTS)
	$(FLAGS) -o $(TARGET) $(OBJECTS) 

helpers.o: helpers.cpp helpers.hpp
	$(FLAGS) -c helpers.cpp

content.o: content.cpp helpers.hpp
	$(FLAGS) -c content.cpp

classes.o: classes.cpp helpers.hpp
	$(FLAGS) -c classes.cpp

main.o: main.cpp helpers.hpp
	$(FLAGS) -c main.cpp