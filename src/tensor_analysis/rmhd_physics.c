#include "rmhd_physics.h"
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------
// 1. Recover Primitive Variables
// ---------------------------------------------------------
int recover_primitives_nr(double D, const double M[3], double E, const double B[3],
                            double gamma_eos, double *rho, double *p, double v[3]) {
    // TODO
    return 0;
}

// ---------------------------------------------------------
// 2. Construct Flat Minkowski Metric and its Inverse
// ---------------------------------------------------------
void construct_minkowski_metric(double g[4][4], double g_inv[4][4]) {
    // TODO
}

// ---------------------------------------------------------
// 3. Compute Lorentz Factor and Contravariant 4-velocity
// ---------------------------------------------------------
void compute_kinematics(const double v[3], double *W, double u[4]) {
    // TODO
}

// ---------------------------------------------------------
// 4. Computes the Covariant Magnetic 4-Field
//    and its Squared Magnitude
// ---------------------------------------------------------
void compute_magnetic_four_vector(const double B[3], const double v[3], double W, 
                                  const double u[4], double b[4], double *b_sq) {
    // TODO
}

// ---------------------------------------------------------
// 5. Compute Specific Enthalpy
// ---------------------------------------------------------
double compute_enthalpy(double rho, double p, double gamma_eos) {
    // TODO
}

// ---------------------------------------------------------
// 6. Calculate Contravariant Components
//    of the Stress-Energy Tensor
// ---------------------------------------------------------
void compute_stress_energy_tensor(double rho, double p, double h, const double u[4], 
                                  const double b[4], double b_sq, 
                                  const double g_inv[4][4], double T[4][4]) {
    // TODO
}

// ---------------------------------------------------------
// 7. Compute Spatial Magnetic Stress Tensor
//    and Magnetization Parameter
// ---------------------------------------------------------
void compute_magnetic_diagnostics(const double B[3], double b_sq, double rho, 
                                  double W, double M[3][3], double *sigma) {
    // TODO
}