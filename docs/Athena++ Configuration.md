## Step 0: Install WSL (if on Windows)

## Step 1: Install Dependencies
sudo apt update

sudo apt install build-essential openmpi-bin libopenmpi-dev libhdf5-openmpi-dev git python3 python3-pip

## Step 2: Clone Athena++
git clone https://github.com/PrincetonUniversity/athena.git

cd athena

## Step 3: Configure for RMHD and MPI
python3 configure.py --prob=blast -b -s --eos=adiabatic --flux=hlle -mpi -hdf5 --include=/usr/include/hdf5/openmpi --lib_path=/usr/lib/x86_64-linux-gnu/hdf5/openmpi

## Step 4: Compile and Run
make clean

make all -j4  # Compiles using 4 cores

mpirun -np 4 ./bin/athena -i inputs/hydro/athinput.blast <!-- OR: mpirun -np 4 ./bin/athena -i athinput.blast -->