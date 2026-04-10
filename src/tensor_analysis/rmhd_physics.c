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
    /* Flat Minkowski Metric (The Inverse is the same):
        g_{\mu v} = [[-1 0 0 0],        
                    [0 1 0 0],
                    [0 0 1 0],
                    [0 0 0 1]]
    */
    // TODO
}

// ---------------------------------------------------------
// 3. Compute Lorentz Factor and Contravariant 4-velocity
// ---------------------------------------------------------
void compute_kinematics(const double v[3], double *W, double u[4]) {
    /* Lorentz Factor and 4-Velocity
        * Given 3-velocity, v = (v_x, v_y, v_z):
        (1) Compute v^2:
            v^2 = v_x^2 + v_y^2 + v_z^2
        (2) Compute Lorentz Factor (W):
            W = 1/sqrt(1-v^2)
        (3) Compute Contravariant 4-velocity:
            u_0 = W,  u_1 = Wv_x,  u_2 = Wv_y,  u_3 = Wv_z
            Alternatively, u^\mu = (W, Wv)
    */
    // TODO
}

// ---------------------------------------------------------
// 4. Computes the Covariant Magnetic 4-Field
//    and its Squared Magnitude
// ---------------------------------------------------------
void compute_magnetic_four_vector(const double B[3], const double v[3], double W, 
                                  const double u[4], double b[4], double *b_sq) {
    /* Covariant Magnetic 4-Field
        * Represents magnetic field in the rest frame.
        * Given laboratory magnetic field, B = (B_x, B_y, B_z):
        (1) Compute Dot Product:
            B*v = B^xv_x + B^yv_y + B^zv_z
        (2) Compute Time Component (b^0):
            b^0 = W(B*v)
        (3) Compute Spatial Components:
            b^i = [B^i + (b^0)u^i]/W
                = B^i/W + b^0v^i
        (4) Compute Squared Magnitude:
            b^2 = g_{\mu v}b^\mu b^v
                = -(b^0)^2 + (b^1)^2 + (b^2)^2 + (b^3)^2
    */
    // TODO
}

// ---------------------------------------------------------
// 5. Compute Specific Enthalpy
// ---------------------------------------------------------
double compute_enthalpy(double rho, double p, double gamma_eos) {
    /* Specific Enthalpy
        Equation:
        h = 1 + \epsilon + (p / \rho)

        Simplified:
        h = 1 + (Γp)/[\rho(Γ - 1)]
    */
    // TODO
}

// ---------------------------------------------------------
// 6. Calculate Contravariant Components
//    of the Stress-Energy Tensor
// ---------------------------------------------------------
void compute_stress_energy_tensor(double rho, double p, double h, const double u[4], 
                                  const double b[4], double b_sq, 
                                  const double g_inv[4][4], double T[4][4]) {
    /* Stress-Energy Tensor
        Ideal MHD:
            T^{\mu v} = (\rho h + b^2)u^\mu u^v +
                        (p + 1/2 b^2)g^{\mu v} - b^\mu b^v
        To Solve for Any Component (\mu, v):
            (1) Let P_{tot} = p + 1/2 b^2 (Total Pressure),
            (2) Let h_{tot} = \rho h + b^2 (Total Enthalpy/Energy Density),
            (3) Calculate Component:
                    T^{\mu v} = h_{tot}u^\mu u^v + P_{tot}g^{\mu v} - b^\mu b^v
                Example - T^{00} (Energy Density):
                    T^{00} = h_{tot}(u^0)^2 +  P_{tot}g^{00} - (b^0)^2
                           = (\rho h + b^2)W^2 - (p + 1/2 b^2) - b^\mu b^v
    */
    // TODO
}

// ---------------------------------------------------------
// 7. Compute Spatial Magnetic Stress Tensor
//    and Magnetization Parameter
// ---------------------------------------------------------
void compute_magnetic_diagnostics(const double B[3], double b_sq, double rho, 
                                  double W, double M[3][3], double *sigma) {
    /* Magnetic Diagnostics
        Magnetization Parameter (\sigma):
        * Ratio of magnetic energy density to rest-mass energy density
        \sigma = b^2 / \rho

        Spatial Magnetic Stress Tensor (M^{ij}):
        * In spatial context, the magnetic contribution to stress is
        * extracted from the Maxwell stress tensor:
        M^{ij} = (1/2 b^2)\delta^{ij} - b^i b^j
            (where '\delta^{ij}' is the Kronecker Delta)
    */
    // TODO
}