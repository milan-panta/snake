# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -std=c++20 -Wall -Wextra

# RayLib flags
RAYLIB_FLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = main

# Default target
all: $(EXECUTABLE)

# Linking the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(RAYLIB_FLAGS)

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(RAYLIB_FLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Run target
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Phony targets
.PHONY: all clean run
