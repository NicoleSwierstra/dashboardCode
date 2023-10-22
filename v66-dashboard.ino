#include "display/Fonts.h"
#include "display/display.h"

void setup() {
    Serial.begin(9600);
    String s = String("Hello World!");

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < s.length(); j++){
            int bit = 0;
            while(++bit < 8){
                bool b = fonts::text6by8ND[s[j]][i] >> (9 - bit) & 0x01;
                Serial.print(b ? ' ' : '0');
            }
        }
        Serial.println();
    }
}

void loop() {
    
}
