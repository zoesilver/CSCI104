# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Target executable
TARGET = ./Interpreter

# Source files
SRCS = EndStatement.cpp Interpreter.cpp LetStatement.cpp PrintStatement.cpp ProgramState.cpp PrintAll.cpp Math.cpp GoTo.cpp Return.cpp ifThen.cpp

# Header files
HDRS = LetStatement.h PrintStatement.h ProgramState.h Statement.h EndStatement.h PrintAll.h Math.h GoTo.h Return.h ifThen.h

# Object files
OBJS = $(SRCS:.cpp=.o)


# Include directories 
INCLUDES = -I./

# Linking step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

#for any .o file, make it depend on the same cpp file and make it depend on all the headers
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# Phony targets
.PHONY: clean

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS)

