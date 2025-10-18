#include <stdio.h>

// Função para calcular o Máximo Divisor Comum (MDC) usando o Algoritmo de Euclides.
// Esta função é essencial para o cálculo do MMC.
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função para calcular o Mínimo Múltiplo Comum (MMC) de dois números.
// Utiliza a relação: MMC(a, b) = (a * b) / MDC(a, b)
int mmc(int a, int b) {
    // Se um dos números for 0, o MMC é 0. Mas pelos limites do problema (Ci >= 2),
    // isso não ocorrerá. A verificação é uma boa prática.
    if (a == 0 || b == 0) {
        return 0;
    }
    return (a * b) / mdc(a, b);
}

int main() {
    int n; // Número de chaves
    int limite_ano = 50;

    // Lê o número de chaves
    if (scanf("%d", &n) != 1 || n < 1 || n > 10) {
         // Verificação básica da entrada N
         printf("Número de chaves inválido.\n");
         return 1;
    }

    // Declara um array para armazenar os ciclos das chaves
    int ciclos[n];

    // Lê os ciclos das N chaves
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &ciclos[i]) != 1 || ciclos[i] < 2 || ciclos[i] > 20) {
            // Verificação básica da entrada Ci
            printf("Valor de ciclo inválido.\n");
            return 1;
        }
    }

    // Calcula o MMC de todos os ciclos de forma iterativa
    // Começamos com o primeiro ciclo
    int resultado_mmc = ciclos[0];

    // Itera a partir do segundo ciclo para calcular o MMC cumulativo
    for (int i = 1; i < n; i++) {
        resultado_mmc = mmc(resultado_mmc, ciclos[i]);
    }

    // Verifica se o resultado está dentro do limite de ano estabelecido
    if (resultado_mmc > 0 && resultado_mmc <= limite_ano) {
        printf("%d\n", resultado_mmc);
    } else {
        printf("Nao eh possivel sincronizar as chaves dentro do limite de anos.\n");
    }

    return 0;
}