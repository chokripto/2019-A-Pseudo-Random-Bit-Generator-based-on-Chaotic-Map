// main.c 
#include <stdio.h>
#include "nmgm_prbg.h"

int main() {
    const char* password = "ABIDINE";
    long F = 1000000; // 1 million de bits

    if (generate_bitstream(password, F) == 0) {
        printf("✅ %ld bits générés et enregistrés dans SuitesNMGM.txt\n", F);
    } else {
        fprintf(stderr, "❌ Échec de la génération.\n");
        return 1;
    }

    return 0;
}
