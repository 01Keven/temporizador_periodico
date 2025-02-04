# Semáforo com Raspberry Pi Pico W

#### Visão Geral
Este projeto implementa um semáforo simples usando o Raspberry Pi Pico W. O semáforo é controlado por LEDs (Vermelho, Amarelo e Verde) e alterna entre os estados de sinalização a cada 3 segundos. Utilizando um temporizador repetitivo para fazer a mudança de estados e acionar os LEDs correspondentes.

![2025-02-03-21-40-31](https://github.com/user-attachments/assets/0205a269-f741-4e81-835a-6ac2590834b8)

### Experimento com BitDogLab
- Os leds não aparecem completamente com as cores correspondentes ao do Wokwi pois é necessario uso de biblioteca para manipulação individual das cores dos LEDs.


https://github.com/user-attachments/assets/6d010e41-0902-48fc-9595-f0fc4d09f347



### Estrutura do Código

1. **Bibliotecas Importadas**
   
   ```c
   #include <stdio.h>
   #include "pico/stdlib.h"
   #include "hardware/timer.h"
   ```

   - **`stdio.h`**: Biblioteca padrão de entrada e saída em C, necessária para as funções de `printf`.
   - **`pico/stdlib.h`**: Inclui funções da biblioteca padrão do Raspberry Pi Pico, como `gpio_init`, `gpio_set_dir` e `sleep_ms`.
   - **`hardware/timer.h`**: Biblioteca responsável pelo controle do temporizador, usada para definir o temporizador repetitivo que alterará os estados do semáforo.

2. **Definição dos Pinos dos LEDs**
   
   ```c
   #define LED_VERMELHO 11
   #define LED_AMARELO 12
   #define LED_VERDE 13
   ```

   - São definidos os pinos GPIO do Raspberry Pi Pico aos quais os LEDs (Vermelho, Amarelo e Verde) estão conectados.
   - `LED_VERMELHO`, `LED_AMARELO` e `LED_VERDE` são mapeados para os pinos 11, 12 e 13, respectivamente.

3. **Declaração da Variável de Estado**
   
   ```c
   volatile int state = 0; // variável que armazena o estado atual para o semáforo
   ```

   - `state`: variável global que armazena o estado atual do semáforo (0 para vermelho, 1 para amarelo e 2 para verde). O modificador `volatile` é utilizado para garantir que o compilador não otimize o acesso a essa variável, já que ela pode ser alterada a partir de uma função de interrupção (callback).

4. **Função Callback do Temporizador**
   
   ```c
   bool repeating_timer_callback(struct repeating_timer *t)
   ```

   - Esta função é chamada a cada 3 segundos, conforme definido pelo temporizador repetitivo.
   - **Desliga todos os LEDs**: Antes de acender um novo LED, todos os LEDs são desligados para garantir que apenas um LED esteja aceso de cada vez.
   
   ```c
   gpio_put(LED_VERMELHO, 0);
   gpio_put(LED_AMARELO, 0);
   gpio_put(LED_VERDE, 0);
   ```

   - **Alternância de estados do semáforo**:
   
   ```c
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
   ```

   - A função altera o estado do semáforo com base no valor de `state`:
     - **Caso 0**: Acende o LED vermelho e imprime a mensagem "Sinal vermelho".
     - **Caso 1**: Acende o LED amarelo e imprime a mensagem "Sinal amarelo".
     - **Caso 2**: Acende o LED verde e imprime a mensagem "Sinal verde".
   
   - Após o estado ser alterado, a variável `state` é atualizada para o próximo valor na sequência (0 -> 1 -> 2 -> 0), garantindo que a sequência do semáforo continue em um loop cíclico.
   
   ```c
   state = (state + 1) % 3;
   ```

   - **Retorno**: A função retorna `true` para garantir que o temporizador continue sendo chamado a cada 3 segundos.

5. **Função Principal**
   
   ```c
   int main()
   {
       stdio_init_all();
   ```

   - **Inicialização do sistema de entrada e saída (stdio)**: `stdio_init_all()` é chamado para inicializar a biblioteca padrão de entrada e saída, permitindo o uso de `printf` no console.

   ```c
       // Configuração dos pinos GPIO como saída
       gpio_init(LED_VERMELHO);
       gpio_set_dir(LED_VERMELHO, GPIO_OUT);
       gpio_init(LED_AMARELO);
       gpio_set_dir(LED_AMARELO, GPIO_OUT);
       gpio_init(LED_VERDE);
       gpio_set_dir(LED_VERDE, GPIO_OUT);
   ```

   - Os pinos dos LEDs (Vermelho, Amarelo e Verde) são inicializados e configurados como saídas (GPIO_OUT) para que os LEDs possam ser controlados.

   ```c
       // Temporizador repetitivo com intervalo de 3 segundos
       struct repeating_timer timer;
       add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
   ```

   - **Temporizador repetitivo**: Um temporizador repetitivo é configurado com um intervalo de 3000 milissegundos (3 segundos). A função `repeating_timer_callback` será chamada a cada 3 segundos para atualizar o estado do semáforo.

   ```c
       while (true) {
           printf("Semáforo funcionando...\n");
           sleep_ms(1000); // Espera 1 segundo
       }
   }
   ```

   - **Loop principal**: O programa entra em um loop infinito onde imprime a mensagem "Semáforo funcionando..." a cada 1 segundo, garantindo que o programa continue em execução.
   - `sleep_ms(1000)` faz com que o programa espere por 1 segundo antes de imprimir a próxima mensagem.

---

### Explicação do Fluxo de Execução

1. **Inicialização**:
   - O sistema é inicializado com `stdio_init_all()` e os pinos GPIO são configurados como saídas para os LEDs.
   
2. **Configuração do Temporizador**:
   - O temporizador é configurado para disparar a função `repeating_timer_callback` a cada 3 segundos. Esse temporizador ficará ativo durante toda a execução do programa.

3. **Alteração de Estado do Semáforo**:
   - A cada 3 segundos, a função `repeating_timer_callback` será chamada, desligando todos os LEDs e acendendo o LED correspondente ao estado atual do semáforo (vermelho, amarelo ou verde). A função também imprime a mensagem do estado no console.
   
4. **Loop Principal**:
   - O loop principal do programa garante que o semáforo continue funcionando indefinidamente. A mensagem "Semáforo funcionando..." é exibida a cada segundo, enquanto os LEDs são alternados.
