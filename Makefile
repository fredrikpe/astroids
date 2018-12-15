
CXX = g++-7

CXXFLAGS = -O3 -g -Wall -Wextra -Werror -std=c++17

LDFLAGS = -lsfml-window -lsfml-graphics -lsfml-system

RM = rm -f

SOURCES = src-variant/main.cpp src-variant/gameobjects.cpp

EXECUTABLE = asteroids

all: $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) -o astroids $(SOURCES) $(CXXFLAGS) $(LDFLAGS) 

clean:
	$(RM) *.o $(EXECUTABLE)
