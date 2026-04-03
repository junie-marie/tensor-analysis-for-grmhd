# ==========================================
# Makefile for GRMHD Tensor Analysis Engine
# ==========================================

# Compiler Definition
# We use mpicc to automatically handle MPI library linking
CC = mpicc

# Compiler Flags
# -Wall -Wextra: Show all standard warnings
# -g: Include debugging symbols (crucial for finding segfaults)
# -O2: Standard level 2 optimization for performance
CFLAGS = -Wall -Wextra -g -O2

# Ubuntu/WSL Parallel HDF5 Paths
# These tell the compiler exactly where the HDF5-MPI packages live
INCLUDES = -I/usr/include/hdf5/openmpi
LDFLAGS = -L/usr/lib/x86_64-linux-gnu/hdf5/openmpi
LIBS = -lhdf5

# Project Structure
SRCS = main.c io_grid.c rmhd_physics.c
OBJS = $(SRCS:.c=.o)
TARGET = tensor_engine

# ------------------------------------------
# Build Rules
# ------------------------------------------

# The default rule
all: $(TARGET)

# The linking rule: Combines all .o files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# The compilation rule: Compiles each .c file into a .o object file
# $< is the input (.c file), $@ is the output (.o file)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to wipe compiled binaries and object files
clean:
	rm -f $(OBJS) $(TARGET)