#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_VERMELHO 13
#define LED_AMARELO  12
#define LED_VERDE    11

// Tempo de troca de estado do semáforo (em ms)
#define TEMPO_TROCA 3000

// Variável para armazenar o estado atual do semáforo
int estado = 0;

// Função de callback do temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    // Alterna os LEDs conforme o estado do semáforo
    switch (estado) {
        case 0:
            gpio_put(LED_VERMELHO, 1); // Liga o vermelho
            printf("Sinal vermelho! Pare!\n");
            break;
        case 1:
            gpio_put(LED_AMARELO, 1);  // Liga o amarelo
            printf("Sinal amarelo! Atenção!\n");
            break;
        case 2:
            gpio_put(LED_VERDE, 1);    // Liga o verde
            printf("Sinal verde! Siga!\n");
            break;
    }

    // Avança para o próximo estado
    estado = (estado + 1) % 3;
    return true;
}

int main() {
    // Inicializa a UART padrão
    stdio_init_all();

    // Configura os pinos dos LEDs como saída
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AMARELO);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Inicializa o temporizador
    struct repeating_timer timer;
    add_repeating_timer_ms(-TEMPO_TROCA, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (1) {
        printf("Semáforo operando... Estado atual: %d\n", estado);
        sleep_ms(1000); 
    }

    return 0;
}
