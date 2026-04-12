#include "io_grid.h"
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------
// 1. Grid Partitioning
// ---------------------------------------------------------
void partition_2d_grid(Domain2D *domain, int my_rank, int num_ranks) {
    // --- X Dimension Partitioning ---
    int base_x = domain->global_nx / domain->ranks_x;
    int rem_x  = domain->global_nx % domain->ranks_x;

    if (domain->rank_coord_x < rem_x) {
        domain->local_nx = base_x + 1;
        domain->offset_x = domain->rank_coord_x * domain->local_nx;
    } else {
        domain->local_nx = base_x;
        domain->offset_x = (rem_x * (base_x + 1)) + 
                           ((domain->rank_coord_x - rem_x) * base_x);
    }

    // --- Y Dimension Partitioning ---
    int base_y = domain->global_ny / domain->ranks_y;
    int rem_y  = domain->global_ny % domain->ranks_y;

    if (domain->rank_coord_y < rem_y) {
        domain->local_ny = base_y + 1;
        domain->offset_y = domain->rank_coord_y * domain->local_ny;
    } else {
        domain->local_ny = base_y;
        domain->offset_y = (rem_y * (base_y + 1)) + 
                           ((domain->rank_coord_y - rem_y) * base_y);
    }
}


// ---------------------------------------------------------
// 2. PHDF5 File Opening
// ---------------------------------------------------------
hid_t open_hdf5_parallel(const char *filename, MPI_Comm cart_comm, MPI_Info info) {
    // TODO - Cameron
    return;
}

// ---------------------------------------------------------
// 3. PHDF5 Chunk Reading
// ---------------------------------------------------------
herr_t read_hdf5_chunk(hid_t file_id, const char *dataset_name,
                        const Domain2D *domain, double *local_data_buffer) {
    // TODO - Junie
    // Use 'H5Sselect_hyperslab' to have each MPI rank read its
    // specific 100x100 block of the conserved variables,
    // ($D, M, E$), and the magnetic field ($B$)
    return;
}