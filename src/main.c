#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <hdf5.h>
#include "tensor_engine/io_grid.h"
#include "tensor_engine/rmhd_physics.h" 

int main(int argc, char **argv) {
    int rank, size;

    // 1. Initialize the MPI Environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    if (rank == 0) {
        printf("Starting GRMHD Tensor Analysis Engine...\n");
        printf("MPI Environment initialized with %d processes.\n", size);
    }

    // 2. Setup 2D Cartesian Topology
    Domain2D domain;
    domain.global_nx = 512; // To be read/passed in dynamically later
    domain.global_ny = 512;

    int dims[2] = {0, 0}; 
    MPI_Dims_create(size, 2, dims);
    domain.ranks_x = dims[0];
    domain.ranks_y = dims[1];

    MPI_Comm cart_comm;
    int periods[2] = {1, 1}; // Assuming periodic boundaries for now
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);

    int cart_rank, coords[2];
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, 2, coords);
    domain.rank_coord_x = coords[0];
    domain.rank_coord_y = coords[1];

    // Calculate local boundaries (Function lives in io_grid.c)
    partition_2d_grid(&domain, cart_rank, size);

    // 3. TODO: Implement Parallel HDF5 Input (Functions live in io_grid.c)
    // hid_t file_id = open_hdf5_parallel("output.athdf", cart_comm, MPI_INFO_NULL);
    // read_hdf5_chunk(file_id, "rho", &domain, local_rho_buffer);
    
    MPI_Barrier(MPI_COMM_WORLD); 
    if (rank == 0) printf("Data reading complete. Beginning tensor computations...\n");

    // 4. TODO: Tensor Computations (Functions live in rmhd_physics.c)
    // Loop over local_nx * local_ny and pass data to physics functions
    // e.g., compute_stress_energy_tensor(...)

    // 5. TODO: Global Reductions
    // e.g., MPI_Reduce(...)

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Analysis complete.\n");
        printf("Total execution time: %f seconds\n", end_time - start_time);
    }

    // Clean up
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}