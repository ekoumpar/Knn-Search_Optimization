# Compiler
CC = clang

# Compiler flags
CFLAGS = -I../include -I/usr/include/openblas -fcilkplus -pthread -lhdf5
LDFLAGS = -L/usr/local/lib -lmatio -lhdf5 -lopenblas -fcilkplus -lm -pthread

# Source files
SRCS = main.c createMatrix.c printMatrix.c distanceBlas.c quickSelect.c readMatFiles.c knnSearch.c saveMatrix.c readBenchmark.c

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = knn

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
    $(CC) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
    rm -f $(OBJS) $(TARGET)

# Run the program with LD_LIBRARY_PATH set
run: $(TARGET)
    LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET)

# Specify phony targets
.PHONY: all clean run