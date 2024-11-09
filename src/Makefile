# Compiler
CC = gcc

# Compiler flags
CFLAGS = -I../include -I/usr/include/openblas -fopenmp 
LDFLAGS = -L/usr/local/lib   -lmatio

# Source files
SRCS = main.c randomMatrix.c printMatrix.c distanceBlas.c quickSelect.c 

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = knn

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lopenblas -fopenmp -lm # Add -fopenmp here

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Specify phony targets
.PHONY: all clean

