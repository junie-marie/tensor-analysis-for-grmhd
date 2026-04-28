#include "io_grid.h"
#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"

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
herr_t read_hdf5_chunk(hid_t file_id, const char *dataset_name, int var_index,
                        const Domain2D *domain, double *local_data_buffer) {
    // TODO - Junie
    // Use 'H5Sselect_hyperslab' to have each MPI rank read its
    // specific 100x100 block of the conserved variables,
    // ($D, M, E$), and the magnetic field ($B$)

    // "hid_t file_id" represents the dataset 
    // 1. Get the dataspace of the dataset in the file (Filespace)
    hid_t dataset_id  = H5Dopen2(file_id, dataset_name, H5P_DEFAULT);
    hid_t filespace_id = H5Dget_space(dataset_id);

    // 2. Define the hyperslab in the file
    hsize_t start[5] = {
        0,                          // MeshBlock index
        (hsize_t)var_index,         // Variable index (D, M1, M2, etc.)
        0,                          // Z-index
        (hsize_t)domain->offset_y,  // Y-offset (from your partition logic)
        (hsize_t)domain->offset_x   // X-offset (from your partition logic)
    };

    hsize_t count[5] = {
        1,                          // One MeshBlock
        1,                          // One Variable
        1,                          // One Z-slice
        (hsize_t)domain->local_ny,  // Local Y size
        (hsize_t)domain->local_nx   // Local X size
    };

    H5Sselect_hyperslab(filespace_id, H5S_SELECT_SET, start, NULL, count, NULL);

    // 3. Define the memory dataspace (Memspace)
    // This describes how the data will be laid out in your 'local_data_buffer'
    hsize_t mem_dims[2] = {(hsize_t)domain->local_ny, (hsize_t)domain->local_nx};
    hid_t memspace_id = H5Screate_simple(2, mem_dims, NULL);

    // 4. Perform the read operation
    // plist_id is used for parallel I/O
    hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
    
    herr_t status = H5Dread(dataset_id, 
                            H5T_NATIVE_DOUBLE, 
                            memspace_id, 
                            filespace_id, 
                            plist_id, 
                            local_data_buffer);

    // 5. Clean up resources
    H5Sclose(memspace_id);
    H5Sclose(filespace_id);
    H5Dclose(dataset_id);

    return status;
}