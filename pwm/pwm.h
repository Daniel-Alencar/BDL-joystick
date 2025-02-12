
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// d_i deve ser um valor entre 1 a 255
// d_f deve ser um valor entre 1 a 15
// wrap deve ser um valor entre 1 a 65535
void compute_pwm_parameters(
    uint32_t frequency, uint32_t f_clock
) {
    uint32_t d_i, d_f;
    uint32_t wrap;
    bool found = false;

    for (d_i = 1; d_i <= 255; d_i++) {
        for (d_f = 0; d_f <= 15; d_f++) {
            wrap = (uint32_t)(f_clock / ((d_i + (d_f / 16.0)) * frequency)) - 1;
            if (wrap >= 1 && wrap <= 65535) {
                found = true;
                printf("d_i: %d | d_f: %d | wrap: %d\n", d_i, d_f, wrap);
                return;
            }
        }
    }

    if (!found) {
        printf("Não encontrou parâmetros\n");
    }
}

double compute_pwm_frequency(uint32_t f_clock, uint32_t wrap, uint32_t d_i, uint32_t d_f) {
    printf("Frequência calculada: %lf\n", f_clock / ((d_i + d_f / 16.0) * (wrap + 1)));
    return f_clock / ((d_i + d_f / 16.0) * (wrap + 1));
}