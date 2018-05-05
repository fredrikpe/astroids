
CXX = g++

CXXFLAGS = -O0 -g -Wall -Wextra -Werror -std=c++14

LDFLAGS = -lsfml-window -lsfml-graphics -lsfml-system

RM = rm -f

SOURCES = src/main.cpp src/gameobjects.cpp

EXECUTABLE = astroids

all: $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) -o astroids $(SOURCES) $(CXXFLAGS) $(LDFLAGS) 

clean:
	$(RM) *.o $(EXECUTABLE)
