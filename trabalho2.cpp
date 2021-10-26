#include <Arduino.h>
//variáveis fixas
int botao = 9;
int modo_de_operacao = 1;

void setup() {
    //pinos de entrada para o botão e de saida para LEDs
    pinMode(botao, INPUT_PULLUP);

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);

    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
}

void saida_comparador(int val1, int val2, int val3) {
    if (val1 > val2 && val1 > val3)
        digitalWrite(10, HIGH);
    if (val2 > val3 && val2 > val1)
        digitalWrite(11, HIGH);
    if (val3 > val2 && val3 > val1)
        digitalWrite(12, HIGH);
}
 
void saida_adc(int val_4bits) {
    //loop para escrever a tensão lida nos bits de saida a cada click no botão
    for (int i = 0; i < 4; ++i) {
        int valor;
        if ((val_4bits & 1 <<i) != 0) {
            valor = HIGH;
        } else {
            valor = LOW;
        }
        digitalWrite((5+i), valor);
    }
}

void loop() {
    int val1, val2, val3;
    val1 = analogRead(A0);
    val2 = analogRead(A1);
    val3 = analogRead(A2);

    saida_comparador(val1, val2, val3);

    //define o contador do botão, adiciona 1 a cada vez que o botão sofrer um click
    if (digitalRead(botao) == LOW) {
        modo_de_operacao += 1;
        if (modo_de_operacao == 4) 
            modo_de_operacao = 1;

        delay(500);
    }
    // conversão do sinal de 10 bits para 4 bits
    int val_4bits;
    switch (modo_de_operacao) {
        case 1:
            val_4bits = val1 / 64;
            break;

        case 2:
            val_4bits = val2 / 64;
            break;

        case 3:
            val_4bits = val3 / 64;
            break;
    }    
    saida_adc(val_4bits);
}
