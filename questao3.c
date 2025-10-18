#include <stdio.h>
#include <math.h>

// Função para calcular a potência, necessária para a fórmula de sigma.
long long power(int base, int exp) {
    long long res = 1;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

int main() {
    int n;

    // 1. Entrada
    printf("Digite um inteiro N (1 <= N <= 105): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 105) {
        printf("Entrada invalida.\n");
        return 1;
    }

    printf("\nAnalisando o numero: %d\n", n);
    printf("----------------------------------------\n");

    // Caso especial: N = 1
    if (n == 1) {
        printf("Fatoracao Prima:\n");
        printf("N = 1 nao tem fatores primos.\n\n");
        printf("Calculo de tau(1): 1\n");
        printf("Calculo de sigma(1): 1\n");
        printf("----------------------------------------\n");
        printf("Razao de Eficiencia: 1.00\n");
        return 0;
    }

    // Variáveis para os cálculos
    long long tau_n = 1;
    long long sigma_n = 1;
    int temp_n = n; // Copia de N para ser modificada na fatoração

    // 2. Fatoração Prima (Trial Division) e Cálculo de Tau e Sigma
    printf("Fatoracao Prima:\n");
    // Itera por divisores começando em 2
    for (int p = 2; p * p <= temp_n; p++) {
        // Se p é um divisor de temp_n
        if (temp_n % p == 0) {
            int exponente = 0;
            // Conta quantas vezes p divide temp_n
            while (temp_n % p == 0) {
                exponente++;
                temp_n /= p;
            }

            // Imprime o fator e seu expoente
            printf("Fator primo: %d, Expoente: %d\n", p, exponente);

            // Atualiza tau(N) usando a fórmula (a_i + 1)
            tau_n *= (exponente + 1);

            // Atualiza sigma(N) usando a fórmula (p^(a+1) - 1) / (p - 1)
            sigma_n *= (power(p, exponente + 1) - 1) / (p - 1);
        }
    }

    // Se sobrou um número em temp_n, ele é um fator primo maior que sqrt(n)
    // e seu expoente é 1.
    if (temp_n > 1) {
        int exponente = 1;
        printf("Fator primo: %d, Expoente: %d\n", temp_n, exponente);

        // Atualiza tau(N) para este último fator
        tau_n *= (exponente + 1);

        // Atualiza sigma(N)
        // A fórmula (p^(1+1) - 1)/(p-1) simplifica para (p^2-1)/(p-1) = p+1
        sigma_n *= (temp_n + 1);
    }

    printf("\n----------------------------------------\n");

    // 3. Imprime os resultados intermediários
    printf("Calculo de tau(%d): %lld\n", n, tau_n);
    printf("Calculo de sigma(%d): %lld\n", n, sigma_n);
    printf("----------------------------------------\n");

    // 4. Calcula e imprime a Razão de Eficiência
    // É crucial converter um dos operandos para double para forçar a divisão de ponto flutuante
    double razao_eficiencia = (double)sigma_n / tau_n;

    printf("Razao de Eficiencia: %.2f\n", razao_eficiencia);

    return 0;
}