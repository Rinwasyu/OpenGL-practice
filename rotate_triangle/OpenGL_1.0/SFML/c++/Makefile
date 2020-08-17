.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -Wextra -O3
LDFLAGS := -lsfml-window -lsfml-system -lGL -lGLU

all: triangle

clean:
	rm triangle

triangle: triangle.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<