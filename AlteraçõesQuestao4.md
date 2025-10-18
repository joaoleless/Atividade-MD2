🔧 Alterações Implementadas

🔹 Alteração [1] — Linha 10

Função: mdcComPassos
Trecho alterado:

while (b != 0) {


Descrição da modificação:

Adicionada a condição b != 0 no laço while.

Justificativa:

Essa é a condição de parada essencial do Algoritmo de Euclides.
O processo de divisão continua até que o resto se torne zero, sendo o último divisor diferente de zero o valor final do MDC.

🔹 Alteração [2] — Linha 14

Função: mdcComPassos
Trecho alterado:

b = resto;


Descrição da modificação:

Atualização da variável b para receber o valor do resto da divisão anterior.

Justificativa:

Esse passo garante a repetição correta do algoritmo, substituindo o divisor antigo (b) pelo resto, que passa a ser o novo divisor da próxima iteração.

🔹 Alteração [3] — Linha 23

Função: inversoModular
Trecho alterado:

if (mdcComPassos(a, m) != 1) {
    printf("\n   ERRO: O inverso modular de %d (mod %d) nao existe.\n", a, m);
    return -1;
}


Descrição da modificação:

Inserida uma verificação para impedir o cálculo do inverso modular quando a e m não são coprimos.

Justificativa:

O inverso modular só existe se o MDC entre a e m for igual a 1.
Essa checagem previne erros e garante a validade matemática da operação.

🔹 Alteração [4] — Linha 36

Função: inversoModular
Trecho alterado:

if (x1 < 0) {
    x1 += m0;
}


Descrição da modificação:

Incluída uma correção para converter valores negativos do inverso em seus equivalentes positivos.

Justificativa:

O Algoritmo de Euclides Estendido pode gerar coeficientes negativos.
Como na aritmética modular usamos sempre o representante positivo, somar o módulo (m0) normaliza o resultado.

🔹 Alteração [5] — Linha 45

Função: powMod
Trecho alterado:

if (exp % 2 == 1) {
    res = (res * b) % mod;
}


Descrição da modificação:

Adicionada a condição que verifica se o expoente é ímpar (exp % 2 == 1).

Justificativa:

Esse é o passo central da exponenciação rápida.
Quando o expoente é ímpar, a base atual é multiplicada ao resultado acumulado, reduzindo o número total de multiplicações e melhorando o desempenho.

🔹 Alteração [6] — Linha 72

Função: main
Trecho alterado:

int inverso = inversoModular(G, Zn);


Descrição da modificação:

A variável inverso agora armazena o retorno da função inversoModular.

Justificativa:

Para efetuar a divisão modular (H / G), é necessário conhecer o inverso de G módulo Zn.
Essa linha faz exatamente essa chamada e guarda o resultado para uso posterior.

🔹 Alteração [7] — Linha 78

Função: main
Trecho alterado:

long long resultado = powMod(a, x, n1);


Descrição da modificação:

O resultado da exponenciação modular é armazenado na variável resultado.

Justificativa:

Depois de obter o valor de a (resultado da divisão modular), é preciso calcular a^x mod n1.
Essa linha utiliza a função otimizada powMod para realizar esse cálculo com eficiência.
