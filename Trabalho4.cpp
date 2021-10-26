#include <Arduino.h>
#include <LiquidCrystal.h>
#include <TMP36.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
TMP36 myTMP36(A0, 5.0);
//variáveis que serão usadas em escopo global
uint8_t tamanho_vetor;
uint16_t *val_leitura;
uint8_t posicao_atual;
uint16_t qtd_val;
uint16_t qtd_sobrescrito;
//função para somar elementos do vetor
int soma() {
int i_soma, soma_total = 0;
for (i_soma = 0; i_soma < tamanho_vetor; ++i_soma){
soma_total += val_leitura[i_soma];
}
return soma_total;
}
//função para calcular a média, na condição if enquanto o vetor está sendo preenchido
// e o else para quando está complemtamente preenchido
float media() {
int i_media;
float media_total, soma_para_calc_media = 0;
if (qtd_val < tamanho_vetor) {
for (i_media = 0; i_media < qtd_val; ++i_media) {
soma_para_calc_media += val_leitura[i_media];
}
media_total = soma_para_calc_media/qtd_val;
} else {
for (i_media = 0; i_media < tamanho_vetor; ++i_media){
soma_para_calc_media += val_leitura[i_media];
}
media_total = soma_para_calc_media/tamanho_vetor;
}
return media_total;
}
// Definição do TOP do timer
#define TIMER_TOP (46875 - 1)
void setup() {
lcd.begin(16,2);
lcd.clear();
//criação do vetor de alocação dinâmica
tamanho_vetor = random(30, 51);
val_leitura = (uint16_t *) malloc(tamanho_vetor * sizeof(*val_leitura));
posicao_atual = 0;
//garantindo que todos valores do vetor iniciarão em 0
for (uint8_t i = 0; i < tamanho_vetor; ++i) {
val_leitura[i] = 0;
}
//setando variáveis globais em 0
qtd_val = 0;
qtd_sobrescrito = 0;
//pino pwm
pinMode(11, OUTPUT);
// Configuração do Timer 1, utilizado para a contagem dos segundos
// Modo de operação CTC (Clear Timer on Compare Match)
// Prescaler de 1024
TCCR1A = 0b00000000; // Timer/Counter 1 Control Register A
TCCR1B = 0b00001101; // Timer/Counter 1 Control Register B
// Habilita a interrupção quando o timer atinge TOP
TIMSK1 = 0b00000010; // Timer/counter 1 Interrupt MaSK register
// Timer começa em 0
TCNT1 = 0;// Timer/CouNTer 1
// TOP do timer é `TIMER_TOP`
OCR1A = TIMER_TOP; // Output Compare Register 1 A
// habilita todas interrupções
sei();
}
volatile uint8_t intervalo_de_3_segundos = 0;
volatile bool exec_leituras_saidas = true;
// Interrupção que é disparada toda vez que o timer atinge TOP
ISR(TIMER1_COMPA_vect) {
if (intervalo_de_3_segundos == 40){
exec_leituras_saidas = true;
intervalo_de_3_segundos = 0;
}
intervalo_de_3_segundos += 1;
}
void loop(){
if (exec_leituras_saidas == true) {
//faz a leitura da porta analógica e incrementa a variavel de quantidade de valores lidos
val_leitura[posicao_atual] = myTMP36.getTempC();
qtd_val += 1;
// variavel para conversão de sinal pwm
//saída do pwm
int sinal_pwm = (myTMP36.getTempC()*255)/60;
analogWrite(11, sinal_pwm);
//verifica se o valor foi sobescrito
if (qtd_val > tamanho_vetor) {
qtd_sobrescrito += 1;
}
//chama as funções para o calculo da soma e média
int soma_val = soma();
float media_val = media();
//printa o valor de leitura atual com 2 casas decimais no display LCD
lcd.setCursor(0,0);
lcd.print(val_leitura[posicao_atual]);
//printa o penultimo valor de leitura com 2 casas decimais no display LCD
if (posicao_atual == 0){
lcd.setCursor(5,0);
lcd.print(val_leitura[tamanho_vetor-1]);
}else{
lcd.setCursor(5,0);
lcd.print(val_leitura[posicao_atual-1]);
}
//printa a quantidade de valores lidos com 2 casas decimais no display LCD
lcd.setCursor(10,0);
lcd.print(qtd_val);
//printa a quantidade de valores sobrescritos com 2 casas decimais no display LCD
lcd.setCursor(13,0);
lcd.print(qtd_sobrescrito);
//printa a soma dos valores dos vetores com 2 casas decimais no display LCD
lcd.setCursor(1,1);
lcd.print(soma_val);
//printa a media de valores do vetor com 2 casas decimais no display LCD
lcd.setCursor(7,1);
lcd.print(media_val, 2);
//incremeta a variável paraa posição atual no vetor
//se for igual ao tamanho do vetor (o que resultaria no overflow)
//o vetor volta para a primeira posição, 0
posicao_atual += 1;
if (posicao_atual == tamanho_vetor) {
posicao_atual = 0;
}
exec_leituras_saidas = false;
}
}
