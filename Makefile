# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -O2

# Source files
SRCS = main.cpp Server.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = simulator

# Default rule to build the executable
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
