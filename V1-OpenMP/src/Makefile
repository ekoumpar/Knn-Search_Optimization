# Compiler
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -I../include -I/usr/include/openblas -I/usr/include/hdf5 -fopenmp -lhdf5
CXXFLAGS = -I../include -I/usr/include/openblas -fopenmp 
LDFLAGS = -L/usr/local/lib -lmatio -lopenblas -fopenmp -lm -lpthread -lhdf5

# Source files
C_SRCS = createMatrix.c distanceBlas.c quickSort.c readMatFiles.c VPtree.c readBenchmark.c
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
