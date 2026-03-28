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
SRC_DIR = src
SRCS = $(SRC_DIR)/tensor_engine.c
TARGET = tensor_engine

# ------------------------------------------
# Build Rules
# ------------------------------------------

# The default rule that runs when you type 'make'
all: $(TARGET)

# The compilation rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# The clean rule to wipe the compiled binary (run with 'make clean')
clean:
	rm -f $(TARGET)