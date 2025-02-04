#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"


// definição dos pinos para os LEDs
#define LED_VERMELHO 11
#define LED_AMARELO 12
#define LED_VERDE 13

volatile int state = 0; // variavel que armazena o estado atual para o semaforo

// callback do temporizador loop
// executa a mudança de estado do semaforo a cada 3 segundos.

bool repeating_timer_callback(struct repeating_timer *t)
{
    // LEDs desligados
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);
    
    switch (state)
    {
    case 0:
        gpio_put(LED_VERMELHO, 1);
        printf("Sinal vermelho\n");
        break;
    
    case 1:
        gpio_put(LED_AMARELO, 1);
        printf("Sinal amarelo\n");
        break;
    
    case 2:
        gpio_put(LED_VERDE, 1);
        printf("Sinal verde\n");
        break;
    }

    // alterna o state entre 0, 1 e 2 reiniciando a sequencia
    state = (state + 1) % 3;
    return true; // mantem o timer ativo
};


int main()
{
    stdio_init_all();
    

    // configuração dos pinos GPIO como saída
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // temporizador repetitivo com intervalo de 3 segundos
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        printf("Semáforo funcionando...\n");
        sleep_ms(1000); // esperando 1 segundo
    }
}
