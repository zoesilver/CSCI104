# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Executable name
TARGET = rsa

# Source files
SRCS = rsa.cpp

# Object files (automatically generated)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
