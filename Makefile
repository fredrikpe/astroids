
CXX = g++-6

CXXFLAGS = -O3 -Wall -std=c++14

LDFLAGS = -lsfml-window -lsfml-graphics -lsfml-system

RM = rm -f

SOURCES = main.cpp gameobjects.cpp

EXECUTABLE = astroids

all: $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) -o astroids $(SOURCES) $(CXXFLAGS) $(LDFLAGS) 

clean:
	$(RM) *.o $(EXECUTABLE)
