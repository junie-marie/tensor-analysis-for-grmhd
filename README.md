# Tensor Analysis for GRMHD Simulation

This repository focuses on building a custom parallel tensor analysis engine (C+MPI) to assist in general-relativistic magnetohydrodynamics (GRMHD) simulations of jets in active galactic nuclei (AGNs).

This project is part of the CS-6373: High Performance Computing course at The University of Tulsa under Professor Dr. Hale.

## Authors
Juniper-Marie Rahal, Isa Fite, Cameron Walker

## System Architecture

```mermaid
graph TD
    subgraph Phase1 [Phase 1: Relativistic Jet Simulation]
        A[Athena++ Solver] -->|MPI Domain Decomposition| B(Fluid Variable Evolution)
        B -->|Parallel HDF5 Output| C[(HDF5 Datasets)]
    end
    
    style A fill:#620693,stroke:#333,stroke-width:2px
    style C fill:#170693,stroke:#333,stroke-width:2px
```

```mermaid
graph LR
    C[(HDF5 Datasets)] -->|MPI-IO Parallel Read| D
    subgraph Phase2 [Phase 2: SPMD Tensor Analysis Engine]
        D[Data Partitioning across MPI Ranks] --> E[Local Computations: Stress-Energy Tensor, Lorentz Factor]
        E -->|MPI_Reduce / MPI_Allreduce| F[Global Reductions & Aggregation]
        F --> G[Final Metrics, Scaling Data, & Visualizations]
    end
    
    style C fill:#170693,stroke:#333,stroke-width:2px
    style G fill:#066293,stroke:#333,stroke-width:2px
```

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
