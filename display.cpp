#include "display.h"
#include "Arduino.h"

void display::putData(char data, const int16_t* pins) {
    for(int i = 0; i < 8 && pins[i] > -1; i++) //checks to make sure the pin adress isn't -1
        digitalWrite(pins[i], (data >> 7 - i) & 0x01); //pushes the bit out to the pin
}

void display::pulseEnable() {
    digitalWrite(DISPLAY_EN, 1);
    digitalWrite(DISPLAY_EN, 0);
}

void display::home(){
    putData(2, DATAOUT);
    putData(0, CONTOUT);
}

void display::clear(){
    putData(1, DATAOUT);
    putData(0, CONTOUT);
}

void display::initDisplay(){
    //output block
    pinMode(DISPLAY_RS, OUTPUT);
    pinMode(DISPLAY_RW, OUTPUT);
    pinMode(DISPLAY_EN, OUTPUT);
    for(int i = 0; i < 8; i++)
        pinMode(DATAOUT[i], OUTPUT);

    // Inits the display
    putData(0b11111111, DATAOUT);
    putData(0b11100000, CONTOUT);

    // Sets display mode (see datasheet)
    putData(0b00111000, DATAOUT);
    putData(0b00000000, CONTOUT);
    pulseEnable();

    // Sets Display to on and cursor to off
    putData(0b00001100, DATAOUT);
    putData(0b00000000, CONTOUT);
    pulseEnable();

    // Sets entry mode (see datasheet)
    putData(0b00000110, DATAOUT);
    putData(0b00000000, CONTOUT);
    pulseEnable();
}

void display::putChar(char c){
    putData(c, DATAOUT);
    putData(0b10100000, CONTOUT);
}

void display::printString(char* cstr){
    for(int i = 0; cstr[i] != 0; i++){
        putChar(cstr[i]);
    }
}