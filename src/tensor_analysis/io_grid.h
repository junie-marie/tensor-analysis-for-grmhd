#ifndef IO_GRID_H
#define IO_GRID_H

#include <mpi.h>
#include <hdf5.h>

// Struct for Global 2D Grid
typedef struct {
    int global_nx;
    int global_ny;
    int ranks_x;
    int ranks_y;
    int rank_coord_x;
    int rank_coord_y;
    int local_nx;
    int local_ny;
    int offset_x;
    int offset_y;
} Domain2D;

// --- FUNCTION DECLARATIONS ---
void partition_2d_grid(Domain2D *domain, int my_rank, int num_ranks);
hid_t open_hdf5_parallel(const char *filename, MPI_Comm cart_comm, MPI_Info info);
herr_t read_hdf5_chunk(hid_t file_id, const char *dataset_name, const Domain2D *domain, double *local_data_buffer);

#endif // IO_GRID_H