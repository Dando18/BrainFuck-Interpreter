CXX = g++
OPTIMIZATION = -O3
WARNINGS = -Wall -pedantic -Werror
DEBUG =
CXX_STANDARD = -std=c++17
FLAGS = $(OPTIMIZATION) $(WARNINGS) $(DEBUG) $(CXX_STANDARD)

TARGET = bf

all: $(TARGET)

$(TARGET): main.cpp interpreter.hpp compiler.hpp
	$(CXX) $(FLAGS) -o $@ $<

clean:
	rm $(TARGET)