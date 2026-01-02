// nmgm_prbg.h
#ifndef NMGM_PRBG_H
#define NMGM_PRBG_H

#include <stdint.h>

// Fonction d'initialisation (Algorithme 1)
void initialize(const char* password, double* x0, double* y0, double* r0);

// Fonction de génération de bits (Algorithme 2)
int generate_bitstream(const char* password, long F, const char* output_filename);

#endif // NMGM_PRBG_H
