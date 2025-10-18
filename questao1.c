#include <stdio.h>    // Para printf, scanf, fgets
#include <stdlib.h>   // Para malloc, free, llabs, exit
#include <string.h>   // Para strlen, strcmp, strcspn
#include <stdbool.h>  // Para o tipo bool, true, false
#include <math.h>     // Para llabs (embora stdlib.h já tenha)
#include <ctype.h>    // Para toupper

// Definimos um buffer máximo para a mensagem de entrada
#define MAX_MSG_LEN 1024

typedef long long ll;

// ===================================================================
// FUNÇÕES AUXILIARES FUNDAMENTAIS
// ===================================================================

/**
 * Calcula o Máximo Divisor Comum (MDC) de 'a' e 'b' usando o
 * Algoritmo de Euclides.
 */
ll calcularMDC(ll a, ll b, bool verbose) {
    ll original_a = a;
    ll original_b = b;
    
    if (verbose) {
        printf("\n--- Calculando mdc(%lld, %lld) via Algoritmo de Euclides ---\n", a, b);
    }

    // O algoritmo de Euclides funciona com 'a' e 'b' positivos
    a = llabs(a);
    b = llabs(b);

    while (b != 0) {
        ll r = a % b;
        if (verbose) {
            printf("  %lld = %lld * %lld + %lld\n", a, (a / b), b, r);
        }
        a = b;
        b = r;
    }

    if (verbose) {
        printf("--- Resultado mdc(%lld, %lld) = %lld ---\n\n", original_a, original_b, a);
    }
    return a;
}

/**
 * Implementação do Algoritmo Estendido de Euclides (EEA).
 * Encontra x e y tais que a*x + b*y = mdc(a, b).
 */
ll euclidesEstendido(ll a, ll b, ll *x, ll *y, bool verbose) {
    // Caso base
    if (a == 0) {
        *x = 0;
        *y = 1;
        if (verbose) {
            printf("  EEA (Base): a=0, b=%lld -> x=0, y=1, mdc=%lld\n", b, b);
        }
        return b;
    }

    ll x1, y1; // Para armazenar resultados da chamada recursiva
    ll mdc = euclidesEstendido(b % a, a, &x1, &y1, verbose);

    // Atualiza x e y usando os resultados da recursão
    // x = y1 - (b/a) * x1
    // y = x1
    *x = y1 - (b / a) * x1;
    *y = x1;

    if (verbose) {
        printf("  EEA (Passo): a=%lld, b=%lld -> x=%lld, y=%lld\n", a, b, *x, *y);
        printf("    Verificacao: %lld*%lld + %lld*%lld = %lld (mdc=%lld)\n", a, *x, b, *y, (a * (*x) + b * (*y)), mdc);
    }

    return mdc;
}

/**
 * Calcula o inverso modular de 'a' (mod m).
 * Encontra 'D' tal que D * a ≡ 1 (mod m).
 */
ll calcularInversoModular(ll a, ll m) {
    printf("\n--- Calculando Inverso Modular: %lld^-1 (mod %lld) ---\n", a, m);
    printf("Usando Algoritmo Estendido de Euclides para resolver %lld*D + %lld*y = 1\n", a, m);
    
    ll x, y; // x será nosso D
    ll g = euclidesEstendido(a, m, &x, &y, true);

    if (g != 1) {
        printf("ERRO: O inverso modular nao existe pois mdc(%lld, %lld) = %lld != 1.\n", a, m, g);
        return -1; // Inverso não existe
    }

    // Garante que o resultado 'x' esteja no intervalo [0, m-1]
    ll res = (x % m + m) % m;

    printf("  EEA encontrou D (x) = %lld\n", x);
    printf("  Inverso Modular = (%lld %% %lld + %lld) %% %lld = %lld\n", x, m, m, m, res);
    printf("  Verificacao: (%lld * %lld) %% %lld = %lld (Deve ser 1)\n", a, res, m, (a * res) % m);
    printf("--- Inverso Modular D = %lld ---\n\n", res);
    
    return res;
}

/**
 * Teste de primalidade simples (Trial Division).
 */
bool ehPrimo(ll n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/**
 * Calcula (base^exp) % mod usando exponenciação
 * (método binário) e exibe todos os passos.
 */
ll potenciaModularComLogs(ll base, ll exp, ll mod, ll z_n) {
    ll n = mod;
    ll exp_original = exp;
    ll exp_reduzido = exp;

    printf("\n--- Calculando (%lld^%lld) %% %lld ---\n", base, exp_original, n);
    
    // -----------------------------------------------------------------
    // JUSTIFICATIVA E REDUÇÃO DO EXPOENTE (Conforme solicitado)
    // -----------------------------------------------------------------
    if (ehPrimo(n)) {
        // Esta condição NUNCA será verdadeira em RSA (n=p*q)
        exp_reduzido = exp % (n - 1);
        printf("[INFO] O modulo %lld e primo.\n", n);
        printf("[APLICADO] Pequeno Teorema de Fermat: exp = %lld %% (%lld-1) = %lld\n", exp, n, exp_reduzido);
    } else {
        // Esta é a condição padrão para RSA
        ll mdc_base_n = calcularMDC(base, n, false); // mdc silencioso
        if (mdc_base_n == 1) {
            exp_reduzido = exp % z_n;
            printf("[INFO] O modulo %lld e composto. mdc(%lld, %lld) = 1.\n", n, base, n);
            printf("[APLICADO] Teorema de Euler: exp = %lld %% z(n) = %lld %% %lld = %lld\n", exp, exp, z_n, exp_reduzido);
        } else {
            // Caso onde M é múltiplo de p ou q.
            exp_reduzido = exp % z_n;
            printf("[INFO] O modulo %lld e composto. mdc(%lld, %lld) = %lld != 1.\n", n, base, n, mdc_base_n);
            printf("[APLICADO] Teorema da Divisao Euclidiana (Reducao Geral): exp = %lld %% z(n) = %lld %% %lld = %lld\n", exp, exp, z_n, exp_reduzido);
        }
    }

    printf("\nCalculo efetivo: (%lld^%lld) %% %lld\n", base, exp_reduzido, n);
    printf("Iniciando exponenciacao (método binário):\n");

    ll res = 1;
    ll temp_base = base % n;
    ll temp_exp = exp_reduzido;

    // Garante que a base não seja negativa após o módulo
    if (temp_base < 0) temp_base += n;

    while (temp_exp > 0) {
        // Se o expoente é ímpar, multiplica o resultado pela base atual
        if (temp_exp % 2 == 1) {
            printf("  exp=%3lld (impar). res = (res * base) %% n = (%lld * %lld) %% n", temp_exp, res, temp_base);
            res = (res * temp_base) % n;
            printf(" = %lld\n", res);
        } else {
            printf("  exp=%3lld (par).   res = %lld (mantem)\n", temp_exp, res);
        }

        // Eleva a base ao quadrado para a próxima iteração
        printf("                      base = (base * base) %% n = (%lld * %lld) %% n", temp_base, temp_base);
        temp_base = (temp_base * temp_base) % n;
        printf(" = %lld\n", temp_base);

        // Divide o expoente por 2
        temp_exp /= 2;
    }

    printf("--- Resultado Final: %lld ---\n\n", res);
    return res;
}

// ===================================================================
// ETAPA 1: FATORAÇÃO ρ DE POLLARD
// ===================================================================

/**
 * A função de iteração g(x) = (x^2 + 1) mod n para o Pollard's Rho.
 */
ll funcaoG(ll x, ll n) {
    // Para N < 9999, x*x cabe em long long.
    return (x * x + 1) % n;
}

/**
 * Encontra um fator não trivial de 'n' usando o método ρ de Pollard
 * com a otimização de detecção de ciclo de Floyd (tartaruga e lebre).
 */
ll fatorarPollardRho(ll n) {
    printf("\n--- Iniciando Fatoracao Rho de Pollard para N = %lld ---\n", n);
    printf("  Funcao g(x) = (x^2 + 1) %% %lld\n", n);
    printf("  Semente x0 = 2\n");
    printf("  Usando Deteccao de Ciclo de Floyd (x1 = Tartaruga, x2 = Lebre)\n");
    printf("  Buscando d = mdc(|x2 - x1|, %lld) > 1\n", n);
    printf("------------------------------------------------------------------\n");
    // %-Ns = string alinhada à esquerda, %Ns = string/num alinhada à direita
    printf("%5s | %10s | %10s | %12s | %10s\n", "Iter", "x1 (g(x1))", "x2 (g(g(x2))", "|x2 - x1|", "mdc");
    printf("------------------------------------------------------------------\n");

    // Semente inicial x0 = 2
    ll x1 = 2; // Tartaruga
    ll x2 = 2; // Lebre
    ll d = 1;
    int i = 0;

    while (d == 1) {
        i++;
        x1 = funcaoG(x1, n);        // x1 = x_i
        x2 = funcaoG(funcaoG(x2, n), n);  // x2 = x_{2i}

        ll diff = llabs(x1 - x2);
        
        // Chamamos o mdc silencioso para não poluir a tabela
        d = calcularMDC(diff, n, false);

        printf("%5d | %10lld | %10lld | %12lld | %10lld\n", i, x1, x2, diff, d);

        // O algoritmo falha se d == n (ciclo encontrado antes do fator)
        if (d == n) {
            printf("  Falha na fatoracao (d=n). Tente uma semente diferente ou outro N.\n");
            return -1; // Sinaliza falha
        }
    }

    printf("------------------------------------------------------------------\n");
    printf("  Fator nao trivial encontrado: %lld\n", d);
    printf("--- Fim do Pollard's Rho ---\n");
    
    return d;
}

/**
 * Encontra um fator *primo* de 'n'.
 * Chama fatorarPollardRho e, se o fator encontrado for composto,
 * chama a si mesmo recursivamente nesse fator.
 */
ll encontrarFatorPrimo(ll n) {
    if (ehPrimo(n)) {
        printf("O numero %lld ja e primo.\n", n);
        return n;
    }

    ll factor = n;
    // Tenta encontrar um fator primo. Se pollardRho retornar um fator
    // composto (ex: 6), o loop tentará fatorar o 6.
    while (!ehPrimo(factor)) {
        factor = fatorarPollardRho(factor);
        if (factor == -1) return -1; // Falha
        
        if (!ehPrimo(factor)) {
            printf("\nO fator %lld encontrado e composto. Fatorando %lld...\n", factor, factor);
        }
    }
    
    printf("\nFator primo %lld confirmado para N=%lld\n", factor, n);
    return factor;
}


// ===================================================================
// ETAPA 3: FUNÇÕES DE PRÉ-CODIFICAÇÃO
// ===================================================================

/**
 *Converte um caractere para seu código numérico (A=11..Z=36, ' '=0).
 */
int caractereParaInteiro(char c) {
    c = toupper(c);
    if (c == ' ') return 0;
    if (c >= 'A' && c <= 'Z') {
        return (c - 'A') + 11;
    }
    return 0; // Trata caracteres desconhecidos (pontuação) como espaço
}

/**
 * Converte um código numérico de volta para um caractere.
 */
char inteiroParaCaractere(ll i) {
    if (i == 0) return ' ';
    if (i >= 11 && i <= 36) {
        return (i - 11) + 'A';
    }
    return '?'; // Bloco decifrado inválido
}

/**
 * Converte uma string de texto em um array (alocado dinamicamente)
 * de blocos numéricos.
 */
ll* stringParaBlocos(const char* mensagem, size_t *num_blocks) {
    *num_blocks = strlen(mensagem);
    ll *blocos = (ll*)malloc(*num_blocks * sizeof(ll));
    if (blocos == NULL) {
        printf("ERRO FATAL: Falha na alocacao de memoria (stringParaBlocos).\n");
        exit(1);
    }
    
    for (size_t i = 0; i < *num_blocks; ++i) {
        blocos[i] = caractereParaInteiro(mensagem[i]);
    }
    return blocos;
}

/**
 * Converte um array de blocos numéricos de volta para uma string
 * (alocada dinamicamente).
 */
char* blocosParaString(const ll* blocos, size_t num_blocks) {
    // +1 para o caractere nulo ('\0')
    char *str = (char*)malloc((num_blocks + 1) * sizeof(char));
    if (str == NULL) {
        printf("ERRO FATAL: Falha na alocacao de memoria (blocosParaString).\n");
        exit(1);
    }
    
    for (size_t i = 0; i < num_blocks; ++i) {
        str[i] = inteiroParaCaractere(blocos[i]);
    }
    str[num_blocks] = '\0'; // Adiciona o terminador nulo
    return str;
}

// ===================================================================
// FUNÇÃO PRINCIPAL (main)
// ===================================================================

int main() {
   
    printf("==========================================================\n");
    printf("PROJETO DE PROGRAMACAO: SISTEMA RSA COM FATORACAO p-POLLARD\n");
    printf("==========================================================\n");

    ll N1 = 0, N2 = 0;
    ll p = 0, q = 0;
    
    // Variáveis para a Etapa 3
    char mensagem_original[MAX_MSG_LEN];
    size_t num_blocos = 0;
    ll *blocos_M = NULL;
    ll *blocos_C = NULL;
    ll *blocos_decifrados = NULL;
    char *mensagem_final = NULL;
    char *mensagem_original_normalizada = NULL;


    // -----------------------------------------------------------------
    // ETAPA 1: FATORAÇÃO
    // -----------------------------------------------------------------
    printf("\nETAPA 1: FATORACAO p DE POLLARD\n");
    printf("---------------------------------\n");
    printf("Cada Ni deve ser um produto de primos distintos (ex: 187=11*17, 799=17*47).\n");
    
    // Validação da entrada N1
    while (N1 < 100 || N1 > 9999) {
        printf("Digite o primeiro numero composto N1 (100 a 9999): ");
        scanf("%lld", &N1);
        if (N1 < 100 || N1 > 9999) {
            printf("Entrada invalida. Tente novamente.\n");
        }
    }

    // Validação da entrada N2
    while (N2 < 100 || N2 > 9999 || N2 == N1) {
        printf("Digite o segundo numero composto N2 (100 a 9999, diferente de N1): ");
        scanf("%lld", &N2);
        if (N2 < 100 || N2 > 9999) {
            printf("Entrada invalida. Tente novamente.\n");
        }
        if (N2 == N1) {
            printf("N2 deve ser diferente de N1. Tente novamente.\n");
        }
    }

    p = encontrarFatorPrimo(N1);
    if (p == -1) return 1; // Encerra se Pollard falhar

    q = encontrarFatorPrimo(N2);
    if (q == -1) return 1; // Encerra se Pollard falhar

    if (p == q) {
        printf("\n*** AVISO IMPORTANTE ***\n");
        printf("Os fatores primos encontrados sao iguais (p = q = %lld).\n", p);
        printf("O calculo do Totiente de Euler z(n) = (p-1)(q-1) assume p != q.\n");
        printf("Para p=q, o correto seria z(n) = p(p-1).\n");
        printf("O programa continuara, mas a seguranca do RSA esta comprometida\n");
        printf("e a decodificacao pode falhar. Recomenda-se reiniciar com\n");
        printf("N1 e N2 que gerem primos distintos.\n");
        printf("Pressione Enter para continuar mesmo assim...");
        // Limpa o buffer de entrada
        while ((getchar()) != '\n'); // Limpa o \n do scanf
        getchar(); // Aguarda o Enter
    }

    printf("\n==============================================\n");
    printf("ETAPA 1 CONCLUIDA: Primos RSA Definidos\n");
    printf("  p (de N1=%lld) = %lld\n", N1, p);
    printf("  q (de N2=%lld) = %lld\n", N2, q);
    printf("==============================================\n");

    // -----------------------------------------------------------------
    // ETAPA 2: GERAÇÃO DAS CHAVES
    // -----------------------------------------------------------------
    printf("\nETAPA 2: GERACAO DAS CHAVES RSA\n");
    printf("---------------------------------\n");
    
    ll n = p * q;
    printf("Calculo do Modulo: n = p * q = %lld * %lld = %lld\n", p, q, n);

    ll z_n = (p - 1) * (q - 1);
    printf("Calculo do Totiente de Euler: z(n) = (p-1) * (q-1) = %lld * %lld = %lld\n", (p-1), (q-1), z_n);

    printf("\nEscolha do Expoente Publico E:\n");
    printf("Buscando o menor E > 1 e E < z(n) tal que mdc(E, z(n)) = 1...\n");
    
    ll E = 2;
    while (E < z_n) {
        printf("Testando E = %lld...\n", E);
        if (calcularMDC(E, z_n, false) == 1) {
            printf("  mdc(%lld, %lld) = 1. Econtrado!\n", E, z_n);
            // Mostra o cálculo verbose do mdc apenas para o E escolhido
            calcularMDC(E, z_n, true);
            break;
        } else {
             printf("  mdc(%lld, %lld) != 1. Tentando proximo...\n", E, z_n);
        }
        E++;
    }

    if (E == z_n) {
        printf("ERRO: Nao foi possivel encontrar um expoente E coprimo de %lld\n", z_n);
        return 1;
    }
    
    printf("Expoente Publico E = %lld\n", E);

    printf("\nCalculo do Expoente Privado D:\n");
    printf("Buscando D tal que D * E = 1 (mod z(n)) => D * %lld = 1 (mod %lld)\n", E, z_n);
    
    ll D = calcularInversoModular(E, z_n);
    if (D == -1) return 1; // Encerra se o inverso não existir

    printf("Expoente Privado D = %lld\n", D);

    printf("\n==============================================\n");
    printf("ETAPA 2 CONCLUIDA: Chaves Geradas\n");
    printf("  Chave Publica (n, E): (%lld, %lld)\n", n, E);
    printf("  Chave Privada (n, D): (%lld, %lld)\n", n, D);
    printf("==============================================\n");


    // -----------------------------------------------------------------
    // ETAPA 3: CODIFICAÇÃO E DECODIFICAÇÃO
    // -----------------------------------------------------------------
    printf("\nETAPA 3: CRIPTOGRAFIA E DESCRIPTOGRAFIA\n");
    printf("-----------------------------------------\n");

    printf("Pre-codificacao: A=11, B=12, ..., Z=36, Espaco=00\n");

    // Limpa o buffer de entrada antes de ler a string
    while ((getchar()) != '\n'); 
    
    printf("Digite a mensagem para criptografar (apenas letras e espacos): ");
    if (fgets(mensagem_original, MAX_MSG_LEN, stdin) == NULL) {
        printf("Erro ao ler a mensagem.\n");
        return 1;
    }
    // Remove o '\n' que o fgets captura
    mensagem_original[strcspn(mensagem_original, "\n")] = 0;

    // 1. Pré-codificação
    blocos_M = stringParaBlocos(mensagem_original, &num_blocos);
    
    printf("\nMensagem original: \"%s\"\n", mensagem_original);
    printf("Blocos numericos (M): ");
    for (size_t i = 0; i < num_blocos; i++) {
        // Imprime com 2 dígitos (ex: 00 para espaço)
        printf("%02lld ", blocos_M[i]);
    }
    printf("\n");

    // 2. Codificação (Criptografia)
    printf("\n--- INICIANDO CRIPTOGRAFIA (C = M^E mod n) ---\n");
    // Aloca memória para os blocos cifrados
    blocos_C = (ll*)malloc(num_blocos * sizeof(ll));
    if (blocos_C == NULL) {
        printf("ERRO FATAL: Falha na alocacao de memoria (blocos_C).\n");
        free(blocos_M); // Libera o que já foi alocado
        return 1;
    }
    
    for (size_t i = 0; i < num_blocos; i++) {
        printf("Criptografando bloco M = %lld...\n", blocos_M[i]);
        ll C = potenciaModularComLogs(blocos_M[i], E, n, z_n);
        blocos_C[i] = C;
    }

    printf("\n--- CRIPTOGRAFIA CONCLUIDA ---\n");
    printf("Mensagem Cifrada (Blocos C): ");
    for (size_t i = 0; i < num_blocos; i++) {
        printf("%lld ", blocos_C[i]);
    }
    printf("\n");


    // 3. Decodificação (Descriptografia)
    printf("\n--- INICIANDO DESCRIPTOGRAFIA (M' = C^D mod n) ---\n");
    // Aloca memória para os blocos decifrados
    blocos_decifrados = (ll*)malloc(num_blocos * sizeof(ll));
    if (blocos_decifrados == NULL) {
        printf("ERRO FATAL: Falha na alocacao de memoria (blocos_decifrados).\n");
        free(blocos_M);
        free(blocos_C);
        return 1;
    }

    for (size_t i = 0; i < num_blocos; i++) {
        printf("Descriptografando bloco C = %lld...\n", blocos_C[i]);
        ll M_decifrado = potenciaModularComLogs(blocos_C[i], D, n, z_n);
        blocos_decifrados[i] = M_decifrado;
    }
    
    printf("\n--- DESCRIPTOGRAFIA CONCLUIDA ---\n");
    printf("Blocos Decifrados (M'): ");
    for (size_t i = 0; i < num_blocos; i++) {
        printf("%02lld ", blocos_decifrados[i]);
    }
    printf("\n");

    // 4. Re-codificação para texto
    mensagem_final = blocosParaString(blocos_decifrados, num_blocos);
    printf("Mensagem Final: \"%s\"\n", mensagem_final);

    // 5. Verificação Final
    printf("\n==============================================\n");
    printf("ETAPA 3 CONCLUIDA: VERIFICACAO\n");
    printf("--------------------------------\n");
    
    // Converte a mensagem original (que pode ter minúsculas/pontuação)
    // para a mesma base de comparação (maiúsculas/espaços)
    mensagem_original_normalizada = blocosParaString(blocos_M, num_blocos);
    printf("Original (convertida): \"%s\"\n", mensagem_original_normalizada);
    printf("Decifrada:             \"%s\"\n", mensagem_final);

    if (strcmp(mensagem_original_normalizada, mensagem_final) == 0) {
        printf("\nSUCESSO: A mensagem decifrada e identica a mensagem original.\n");
    } else {
        printf("\nFALHA: A mensagem decifrada e DIFERENTE da original.\n");
    }
    printf("==============================================\n");

    // Libera toda a memória alocada dinamicamente
    free(blocos_M);
    free(blocos_C);
    free(blocos_decifrados);
    free(mensagem_final);
    free(mensagem_original_normalizada);

    return 0;
}