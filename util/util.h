#include <ctype.h>

void extract_numbers(const char *str, int *numbers, int *count) {
    // Inicializa a contagem de números encontrados
    *count = 0;
  
    while (*str) {
      if (isdigit(*str)) {
        // Converte char para inteiro
        numbers[*count] = *str - '0';
        (*count)++;
      }
      str++;
    }
  }