#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <hdf5.h>
#include "tensor_engine/io_grid.h"
#include "tensor_engine/rmhd_physics.h"

#include "tensor_engine/io_grid.c"
#include "tensor_engine/rmhd_physics.c"

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

    /*
    3. TODO: Implement Parallel HDF5 I/O (Functions live in io_grid.c)
    Allocating local buffers
    int local_size = domain.local_nx * domain.local_ny;
    double *buff_D  = malloc(local_size * sizeof(double));
    double *buff_M1 = malloc(local_size * sizeof(double));
    double *buff_M2 = malloc(local_size * sizeof(double));
    double *buff_M3 = malloc(local_size * sizeof(double));
    double *buff_E  = malloc(local_size * sizeof(double));
    double *buff_B1 = malloc(local_size * sizeof(double));
    double *buff_B2 = malloc(local_size * sizeof(double));
    double *buff_B3 = malloc(local_size * sizeof(double));

    // Reading File(s)
    hid_t file_cons = open_hdf5_parallel("blast.out1.00010.athdf", cart_comm, MPI_INFO_NULL);
    read_hdf5_chunk(file_cons, "cons", 0, &domain, buff_D);
    read_hdf5_chunk(file_cons, "cons", 1, &domain, buff_E);
    read_hdf5_chunk(file_cons, "cons", 2, &domain, buff_M1);
    read_hdf5_chunk(file_cons, "cons", 3, &domain, buff_M2);
    read_hdf5_chunk(file_cons, "cons", 4, &domain, buff_M3);
    H5Fclose(file_cons);
    
    MPI_Barrier(MPI_COMM_WORLD); 
    if (rank == 0) printf("Data reading complete. Beginning tensor computations...\n");
    */

    /*
    4. TODO: Tensor Computations (Functions live in rmhd_physics.c)
    Loop over local_nx * local_ny and pass data to physics functions
    e.g., compute_stress_energy_tensor(...)
    
    double local_max_sigma = 0.0;

        for (int j = 0; j < domain.local_ny; j++) {
            for (int i = 0; i < domain.local_nx; i++) {
                int idx = j * domain.local_nx + i;

                Extract values for this cell
                double M[3] = {buff_M1[idx], buff_M2[idx], buff_M3[idx]};
                double B[3] = {buff_B1[idx], buff_B2[idx], buff_B3[idx]};
            
                Physics Pipeline
                double rho, p, v[3], W, u[4], b[4], b_sq, h, T[4][4], g[4][4], g_inv[4][4];
                double M_diag[3][3], sigma;

                A. Recover Primitives from Conserved
                recover_primitives_nr(buff_D[idx], M, buff_E[idx], B, 1.333, &rho, &p, v);

                B. Geometric and Kinematic quantities
                construct_minkowski_metric(g, g_inv);
                compute_kinematics(v, &W, u);
                compute_magnetic_four_vector(B, v, W, u, b, &b_sq);
                h = compute_enthalpy(rho, p, 1.333);

                C. Final Tensor and Diagnostics
                compute_stress_energy_tensor(rho, p, h, u, b, b_sq, g_inv, T);
                compute_magnetic_diagnostics(B, b_sq, rho, W, M_diag, &sigma);

                if (sigma > local_max_sigma) local_max_sigma = sigma;
           }
       }
    */

    /*
       5. TODO: Global Reductions
       e.g., MPI_Reduce(...)
       double global_max_sigma;
       MPI_Reduce(&local_max_sigma, &global_max_sigma, 1, MPI_DOUBLE, MPI_MAX, 0, cart_comm);

       if (rank == 0) printf("Global Maximum Magnetization (Sigma): %f\n", global_max_sigma);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Analysis complete.\n");
        printf("Total execution time: %f seconds\n", end_time - start_time);
    }
    */

    // Clean up
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}