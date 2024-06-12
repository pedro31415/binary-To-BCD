#include <msp430g2553.h>

// Função para criar um atraso em milissegundos
void delay_ms(unsigned int ms) {
  while (ms--) {
    __delay_cycles(1000); // 1 ms delay (assumindo 1 MHz clock)
  }
}

// Função para piscar o LED um número específico de vezes
void blink(unsigned int times) {
  for(unsigned int i = 0; i < times; i++){
    P1OUT ^= 0x01; // Toggle P1.0 (LED)
    delay_ms(500); // 500 ms delay
    P1OUT ^= 0x01; // Toggle P1.0 (LED)
    delay_ms(500); // 500 ms delay
  }
  delay_ms(2000);  // 2 segundos de pausa entre os dígitos
}

void binaryToBCD(unsigned int binary, unsigned char *bcd){
  unsigned char i;

  //Inicializar o array BCD com 0
  for(i = 0; i < 4; i++){
    bcd[i] = 0;
  }

  // Para cada bit no número binário (até 16 bits)
  // Desloca cada dígito BCD à esquerda
  for(i = 0; i < 16; i++){
     for(int j = 3; j > 0; j--){
      bcd[j] <<= 1;
       if(bcd[j - 1] & 0x10) {
        bcd[j] |= 1;
       }
     }

     bcd[0] <<= 1;
     if(binary & 0x8000) {
        bcd[0] |= 1;
     }
     binary <<= 1;
     // Adiciona 3 aos dígitos BCD que são maiores ou iguais a 5
     for(int j = 0; j < 4; j++){
        if(bcd[j] >= 5){
            bcd[j] += 3;
        }
     }
  }
}

int main(void){
   WDTCTL = WDTPW | WDTHOLD; // Parar o watchdog timer

    P1DIR |= 0x01; // Configurar P1.0 como saída (LED)

    unsigned int binary = 185; // Exemplo de número binário
    unsigned char bcd[4]; // Array para armazenar o resultado BCD

    // Converter o número binário para BCD
    binaryToBCD(binary, bcd);

    // Piscar o LED para indicar o valor BCD
    while (1) {
        blink(bcd[1]);  // Piscar para o dígito das centenas
        blink(bcd[2]);  // Piscar para o dígito das dezenas
        blink(bcd[3]);  // Piscar para o dígito das unidades
    }
}