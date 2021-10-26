#include <Arduino.h>
#include <IRremote.h>
#define receptor 11
#define rele 7
IRrecv recIR(receptor);
decode_results resultado;
void setup() {
recIR.enableIRIn(); //inicializa o receptor IR
pinMode(rele, OUTPUT);
}
bool estado_do_rele = LOW;
void loop(){
if(recIR.decode(&resultado)){
if(resultado.value == 0xFF906F){
estado_do_rele = !estado_do_rele;
digitalWrite(rele, estado_do_rele);
}
recIR.resume();
}
}
