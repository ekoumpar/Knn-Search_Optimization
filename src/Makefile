# Compiler
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -I../include -I/usr/include/openblas -fopenmp 
CXXFLAGS = -I../include -I/usr/include/openblas -fopenmp 
LDFLAGS = -L/usr/local/lib -lmatio -lopenblas -fopenmp -lm -lpthread

# Source files
C_SRCS = randomMatrix.c printMatrix.c distanceBlas.c quickSelect.c readMatFiles.c VPtree.c
CPP_SRCS = main.cpp searchVPtree.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)
OBJS = $(C_OBJS) $(CPP_OBJS)

# Target executable
TARGET = knn

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Pattern rules for compiling C and C++ source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program with LD_LIBRARY_PATH set
run: $(TARGET)
	LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET)

# Specify phony targets
.PHONY: all clean run