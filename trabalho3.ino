#include <stdlib.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int tamanho = random(30, 50);
void setup(){
    lcd.begin(16,2);
    lcd.clear();
}

void sum(int val_leitura, int i){
    //soma dos valores
    int i_sum, soma_sum;
    for (i_sum=0, i, pos++){
        soma_sum += val_leitura[i_sum];
        //lcd mostra a soma
    }
    return sum;
}

void media(int val_leitura, int i){
    if (i<=tamanho){
        int sum_media, i_media;
        for (i_media = 0, i, i_media++){
            sum_media += val_leitura[i_media];
        }
        int media = sum_media/i
    } else{
        int sum_media, i_media;
        for (i_media = 0, i, i_media++){
            sum_media += val_leitura[i_media];
        }
        int media = sum_media/tamanho;
    }
}

void loop(){
    //vetor malloc para as leituras
    int *val_leitura;
    val_leitura = (int*)malloc(tamanho*sizeof(int));

    //leitura da porta analógica
    int analogPort;
    int i;
    for (i=0, (tamanho-1), i++){
        val_leitura[i] = analogReaf(A0);
        media(val_leitura, i);
        sum(val_leitura, i);
        int qtd++;
        //lcd print qtd
    }




    lcd.setCursor(3,0);
    lcd.print("OLA");
    lcd.setCursor(4,1);
    lcd.print("DELICIA");
}