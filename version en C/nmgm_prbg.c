// nmgm_prbg.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "nmgm_prbg.h"

// -------------------------------
// Algorithme 1: Initialisation depuis mot de passe
// -------------------------------
void initialize(const char* password, double* x0, double* y0, double* r0) {
    if (!password || strlen(password) == 0) {
        fprintf(stderr, "Error: password is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Conversion en binaire (ASCII)
    int n = strlen(password) * 8;
    char* Pw = (char*)malloc(n);
    for (int i = 0; i < strlen(password); ++i) {
        unsigned char c = password[i];
        for (int j = 0; j < 8; ++j) {
            Pw[i * 8 + j] = (c >> (7 - j)) & 1;
        }
    }

    // Initialisation du pointeur Z
    long Z = n / 4;
    uint64_t A = 0, B = 0, C = 0;

    // Extraire 64 bits pour A, B, C avec le pointeur linéaire congruentiel
    for (int i = 0; i < 64; ++i) {
        Z = (((long long)n * n + 1) * Z + 1) % n;
        int bit = Pw[Z];

    }

    // REFACTORISATION : 192 bits distincts
    free(Pw);
    Pw = (char*)malloc(n);
    for (int i = 0; i < strlen(password); ++i) {
        unsigned char c = password[i];
        for (int j = 0; j < 8; ++j) {
            Pw[i * 8 + j] = (c >> (7 - j)) & 1;
        }
    }

    Z = n / 4;
    for (int i = 0; i < 64; ++i) {
        Z = (((long long)n * n + 1) * Z + 1) % n;
        A |= ((uint64_t)Pw[Z] << (63 - i));
    }
    for (int i = 0; i < 64; ++i) {
        Z = (((long long)n * n + 1) * Z + 1) % n;
        B |= ((uint64_t)Pw[Z] << (63 - i));
    }
    for (int i = 0; i < 64; ++i) {
        Z = (((long long)n * n + 1) * Z + 1) % n;
        C |= ((uint64_t)Pw[Z] << (63 - i));
    }

    *x0 = (double)A / pow(2.0, 63);
    *y0 = (double)B / pow(2.0, 63);
    *r0 = (double)C / pow(2.0, 63) + 2.0;

    free(Pw);
}

// -------------------------------
// Carte NMGM modifiée
// -------------------------------
void nmgm_map(double x, double y, double r, double* x_next, double* y_next) {
    *x_next = 1.0 - y + fabs(x);
    *y_next = x + r * sin(y);
}

// -------------------------------
// Algorithme 2: Génération avec sauvegarde dans SuitesNMGM.txt
// -------------------------------
int generate_bitstream(const char* password, long F) {
    double x, y, r;
    initialize(password, &x, &y, &r);

    int n = strlen(password) * 8; // longueur de la clé en bits
    long M = (long)(r * n);       // période transitoire (n0)

    FILE* fout = fopen("SuitesNMGM.txt", "w");
    if (!fout) {
        perror("Impossible d'ouvrir SuitesNMGM.txt");
        return -1;
    }

    long i = 0;
    long bits_written = 0;

    while (bits_written < F) {
        double x_next, y_next;
        nmgm_map(x, y, r, &x_next, &y_next);
        x = x_next;
        y = y_next;

        if (i >= M) {
            if ((i % n) != 0) {
                // Extraire les parties fractionnaires
                double x_frac = x - floor(x);
                double y_frac = y - floor(y);

                // Convertir en entiers 32 bits
                uint32_t X = (uint32_t)(x_frac * 4294967296.0); // 2^32
                uint32_t Y = (uint32_t)(y_frac * 4294967296.0);
                uint32_t R = X ^ Y;

                // Écrire les 32 bits de R (du MSB au LSB)
                for (int b = 31; b >= 0 && bits_written < F; --b) {
                    fputc('0' + ((R >> b) & 1), fout);
                    bits_written++;
                }
            } else {
                // Mise à jour de r : r = ((r + 1)^2 mod 2) + 2
                r = fmod((r + 1.0) * (r + 1.0), 2.0) + 2.0;
            }
        }
        i++;
    }

    fclose(fout);
    return 0;
}
