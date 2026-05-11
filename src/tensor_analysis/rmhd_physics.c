#include "rmhd_physics.h"
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------
// 1. Recover Primitive Variables
//      DONE
// ---------------------------------------------------------
int recover_primitives_nr(double D, const double M[3], double E, const double B[3],
                          double gamma_eos, double *rho, double *p, double v[3]) {
    
    // 1. Recover Rest-Mass Density
    *rho = D;
    
    // Safety Check: Density must be strictly positive
    if (*rho <= 0.0) {
        return 1; // Error: Unphysical density
    }

    // 2. Recover 3-Velocity (v^i = M^i / D)
    v[0] = M[0] / D;
    v[1] = M[1] / D;
    v[2] = M[2] / D;

    // 3. Compute Squared Magnitudes
    double v_sq = (v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]);
    double B_sq = (B[0]*B[0]) + (B[1]*B[1]) + (B[2]*B[2]);

    // 4. Compute Energies
    double E_kin = 0.5 * D * v_sq;
    double E_mag = 0.5 * B_sq; // Assuming natural units

    // 5. Recover Gas Pressure
    //    E_tot = E_internal + E_kinetic + E_magnetic
    //    p = E_internal * (Gamma - 1)
    double E_int = E - E_kin - E_mag;
    *p = E_int * (gamma_eos - 1.0);

    // Safety Check: Pressure must be strictly positive
    if (*p <= 0.0) {
        return 2; // Error: Unphysical pressure (requires a floor/fixup later)
    }

    return 0; // Success
}

// ---------------------------------------------------------
// 2. Construct Flat Minkowski Metric and its Inverse
//      DONE
// ---------------------------------------------------------
void construct_minkowski_metric(double g[4][4], double g_inv[4][4]) {
    /* Flat Minkowski Metric (The Inverse is the same):
        g_{\mu v} = [[-1 0 0 0],        
                    [0 1 0 0],
                    [0 0 1 0],
                    [0 0 0 1]]
    */
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if (i == j) {
                g[i][j]     = (i == 0) ? -1.0 : 1.0;
                g_inv[i][j] = (i == 0) ? -1.0 : 1.0;
            } else {
                g[i][j]     = 0.0;
                g_inv[i][j] = 0.0;
            }
        }
    }
}

// ---------------------------------------------------------
// 3. Compute Lorentz Factor and Contravariant 4-velocity
//      DONE
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
    
    // Computing v^2
    double v_squared = pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2);

    // Safety check: prevent unphysical velocities (v >= c)
    // We cap v2 at slightly less than 1.0 to avoid division by zero or NaN
    const double MAX_V2 = 1.0 - 1e-15; 
    if (v_squared > MAX_V2) {
        v_squared = MAX_V2;
    }

    // Computing the Lorentz Factor (W)
    *W = 1.0 / sqrt(1.0 - v_squared);

    // Computing Contravariant 4-Velocity (u^mu)
    u[0] = *W;               // Time component (u^t)
    u[1] = *W * v[0];        // Spatial x component (u^x)
    u[2] = *W * v[1];        // Spatial y component (u^y)
    u[3] = *W * v[2];        // Spatial z component (u^z)
}

// ---------------------------------------------------------
// 4. Computes the Covariant Magnetic 4-Field
//    and its Squared Magnitude
//      DONE
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

    // Computing Dot Product
    double B_dot_v = (B[0]*v[0]) + (B[1]*v[1]) + (B[2]*v[2]);

    // Computing Time Component
    double b0 = B_dot_v*W;

    // Computing Spatial Components
    double b1 = (B[0]/W) + (b0*v[0]);
    double b2 = (B[1]/W) + (b0*v[1]);
    double b3 = (B[2]/W) + (b0*v[2]);

    // Computing Squared Magnitude
    double b_squared = -pow(b0, 2) + pow(b1, 2) + pow(b2, 2) + pow(b3, 2);
}

// ---------------------------------------------------------
// 5. Compute Specific Enthalpy
//      DONE
// ---------------------------------------------------------
double compute_enthalpy(double rho, double p, double gamma_eos) {
    /* Specific Enthalpy
        Equation:
        h = 1 + \epsilon + (p / \rho)

        Simplified:
        h = 1 + (Γp)/[\rho(Γ - 1)]
    */
    double h = 1.0 + (gamma_eos*p)/(rho*(gamma_eos - 1.0));

    return h;
}

// ---------------------------------------------------------
// 6. Calculate Contravariant Components
//    of the Stress-Energy Tensor
//      DONE
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
    double P_tot = p + 0.5*b_sq;
    double h_tot = rho*h+b_sq;
    for(int i=0; i < 4; i++) {
        for(int j=0; j < 4; j++) {
            T[i][j] = (h_tot*u[i]*u[j]) + (P_tot*g_inv[i][j]) - b[i]*b[j];
        }
    }
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
    // Magnetization Parameter:
    *sigma = b_sq / rho;

    for(int i=0; i < 3; i++) {
        for(int j=0; j < 3; j++) {
            // Base calculation: -B^i B^j
            M[i][j] = -(B[i] * B[j]);
            if (i == j) {
                M[i][j] += 0.5 * b_sq;
            } else {
                // Mirror across the diagonal for off-diagonal terms
                M[j][i] = M[i][j];
            }
        }
    }
}
