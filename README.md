# Tensor Analysis for GRMHD Simulation

This repository focuses on building a custom parallel tensor analysis engine (C+MPI) to assist in general-relativistic magnetohydrodynamics (GRMHD) simulations of jets in active galactic nuclei (AGNs).

This project is part of the CS-6373: High Performance Computing course at The University of Tulsa under Professor Dr. Hale.

## Authors
Juniper-Marie Rahal, Isa Fite, Cameron Walker

## Prerequisites
To compile and run the code in this repository, your system must have the following installed:
* **C Compiler:** `gcc` or similar.
* **MPI Library:** OpenMPI or MPICH.
* **HDF5 Library:** Compiled with parallel (MPI) support enabled.
* **Python 3:** For running validation scripts and data visualization (`h5py`, `numpy`, `matplotlib`).

## Building the Engine
*(Instructions to be added once the Makefile is finalized)*

## Running the Athena++ Simulations
We use Athena++ to generate our baseline 2D relativistic jet datasets. Please refer to the `docs/` folder for our specific Athena++ configuration flags and initial conditions.
