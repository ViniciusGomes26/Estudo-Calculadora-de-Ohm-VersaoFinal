#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Função de leitura de caracteres
char ler_caractere() { 
    int c; 
    while (1) {
        c = getchar(); // Permitir a digitação de caracteres
        if (c != EOF && c != '\n' && c != '\r') { // Parâmetros para leitura critica do caracter, sem contar espaços, ou linhas
            return (char)c; // resposta ao usuário 
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

// Função auxiliar para ler de forma mais crítica o float 
float ler_float() {

    char buffer[32]; // Amarzem de limite de caracteres por linha
    int i = 0; 
    int c; // Caractere
    
    while (i < sizeof(buffer) - 1) { // Não permite mais de 31 caracteres
        c = getchar();  // Parâmetros de caractere
        if (c != EOF) { // Codigo confirma que foi escrito pelo usuário        
            if (c == '\n' || c == '\r') {
                if (i > 0) break; // Trava que so abre, após o digito e Enter
                else continue;
            }
            
            putchar(c); // retorno do caracter, para saber que o a resposta foi lida
            
            buffer[i++] = (char)c; // Estabelece uma fila, para calibrar o monitoramento do getchar
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    buffer[i] = '\0'; // Estabelece um fim após o digito, evita memória fantasma
    printf("\n"); // Nova linha após digitar o número
    
    float valor;
    sscanf(buffer, "%f", &valor); // Tradutor de letras 
    return valor;
}

void calculadora_lei_de_ohm () {
    char opcao;
    float tensao, corrente, resistencia;
    float resultado;

    printf("\n--- Calculadora de Lei de Ohm ---\n");
    printf("Escolha a grandeza que deseja encontrar:\n");
    printf("V - Tensao | I - Corrente | R - Resistencia\n");
    
    opcao = ler_caractere();
    printf("Opcao selecionada: %c\n\n", opcao);

    switch (opcao) {
        case 'V':
        case 'v':
            printf("Informe a Resistencia: ");
            resistencia = ler_float();
            
            printf("Informe a Corrente: ");
            corrente = ler_float();
            
            resultado = resistencia * corrente;
            printf("Resultado: Tensao = %.2f V\n", resultado);
            break;

        case 'I':
        case 'i':
            printf("Informe a Tensao: ");
            tensao = ler_float();
            
            printf("Informe a Resistencia: ");
            resistencia = ler_float();
            
            if (resistencia != 0) { 
                resultado = tensao / resistencia;
                printf("Resultado: Corrente = %.2f A\n", resultado);
            } else {
                printf("Erro: Resistencia nao pode ser zero!\n");
            }
            break;

        case 'R':
        case 'r':
            printf("Informe a Tensao: ");
            tensao = ler_float();

            printf("Informe a Corrente: ");
            corrente = ler_float();
            
            if (corrente != 0) { 
                resultado = tensao / corrente;
              
                printf("Resultado: Resistencia = %.2f Ohms\n", resultado); 
            } else {
                printf("Erro: Corrente nao pode ser zero!\n");
            }
            break;

        default:
            printf("Opcao [%c] invalida!\n", opcao);
            break;
    }
    printf("\nReiniciando em 3 segundos...\n");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
}

void app_main() {
  // Controlador de respostas 
    setvbuf(stdin, NULL, _IONBF, 0);  // Entrada (números e caracteres)
    setvbuf(stdout, NULL, _IONBF, 0); // Saídas (informações da tela) 


    while(1) {
        calculadora_lei_de_ohm();
        vTaskDelay(500 / portTICK_PERIOD_MS); 
    }
}