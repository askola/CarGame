INC=-I./../../Box2D_v2.1.2/Box2D 

CC=g++
CXXFLAGS = -c -g -std=c++0x -Wall -Wextra -pedantic $(INC)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system \
-L./../../Box2D_v2.1.2/Box2D/Build/Box2D -lBox2D
SOURCES = $(wildcard *.cpp track/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = TheGame

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 

.cc.o:
	$(CC) $(CXXFLAGS) $< -o $@
	
run:
	make && ./$(EXECUTABLE) 
	
clean:
	make && rm -r *.o && ./$(EXECUTABLE)
	 
