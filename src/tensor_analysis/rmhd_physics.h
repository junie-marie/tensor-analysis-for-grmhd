#ifndef RMHD_PHYSICS_H
#define RMHD_PHYSICS_H

/**
 * 1) Primitive Variable Recovery
 * Applies Newton-Raphson root-finding to invert conserved variables to primitives.
 * @param D         Eulerian rest-mass density.
 * @param M         Momentum vector [3].
 * @param E         Total energy.
 * @param B         Magnetic field vector [3].
 * @param gamma_eos Adiabatic index for the ideal gas equation of state.
 * @param rho       Output: Rest-mass density.
 * @param p         Output: Gas pressure.
 * @param v         Output: Fluid velocity vector [3].
 * @return          Number of iterations taken, or -1 if convergence failed.
 */
int recover_primitives_nr(double D, const double M[3], double E, const double B[3], 
                          double gamma_eos, double *rho, double *p, double v[3]);

/**
 * 2) Metric Tensor
 * Constructs the flat Minkowski metric and its inverse.
 * @param g     Output: Covariant metric tensor [4][4].
 * @param g_inv Output: Contravariant metric tensor [4][4].
 */
void construct_minkowski_metric(double g[4][4], double g_inv[4][4]);

/**
 * 3) Kinematics
 * Computes the Lorentz factor and contravariant 4-velocity.
 * @param v Output: Fluid 3-velocity vector [3].
 * @param W Output: Lorentz factor.
 * @param u Output: 4-velocity vector [4].
 */
void compute_kinematics(const double v[3], double *W, double u[4]);

/**
 * 4) Electromagnetics
 * Computes the covariant magnetic 4-field and its squared magnitude.
 * @param B    Magnetic 3-field vector [3].
 * @param v    Fluid 3-velocity vector [3].
 * @param W    Lorentz factor.
 * @param u    4-velocity vector [4].
 * @param b    Output: Magnetic 4-field vector [4].
 * @param b_sq Output: Squared magnitude of the magnetic 4-field (b^mu b_mu).
 */
void compute_magnetic_four_vector(const double B[3], const double v[3], double W, 
                                  const double u[4], double b[4], double *b_sq);

/**
 * 5) Thermodynamics
 * Computes the specific enthalpy.
 * @param rho       Rest-mass density.
 * @param p         Gas pressure.
 * @param gamma_eos Adiabatic index.
 * @return          Specific enthalpy.
 */
double compute_enthalpy(double rho, double p, double gamma_eos);

/**
 * 6) Stress-Energy Tensor
 * Calculates the contravariant components of the stress-energy tensor.
 * @param rho   Rest-mass density.
 * @param p     Gas pressure.
 * @param h     Specific enthalpy.
 * @param u     4-velocity vector [4].
 * @param b     Magnetic 4-field vector [4].
 * @param b_sq  Squared magnitude of the magnetic 4-field.
 * @param g_inv Contravariant metric tensor [4][4].
 * @param T     Output: Stress-Energy tensor [4][4].
 */
void compute_stress_energy_tensor(double rho, double p, double h, const double u[4], 
                                  const double b[4], double b_sq, 
                                  const double g_inv[4][4], double T[4][4]);

/**
 * 7) Magnetic Diagnostics
 * Computes the spatial magnetic stress tensor and magnetization parameter.
 * @param B     Magnetic 3-field vector [3].
 * @param b_sq  Squared magnitude of the magnetic 4-field.
 * @param rho   Rest-mass density.
 * @param W     Lorentz factor.
 * @param M     Output: Spatial magnetic stress tensor [3][3].
 * @param sigma Output: Magnetization parameter.
 */
void compute_magnetic_diagnostics(const double B[3], double b_sq, double rho, 
                                  double W, double M[3][3], double *sigma);

#endif // RMHD_PHYSICS_H