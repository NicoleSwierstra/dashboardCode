#include "display/Fonts.h"
#include "display/display.h"
#include "dashstorage.h"
#include <Time.h>
#include <FlexCAN_T4.h>
#include "display/VehicleWireframeData.h"

//FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

void display::debugDrawToSerial(){
	for(int y = 0; y < HEIGHT; y += 2) {
		for(int x = 0; x < 8; x++) {
			std::string s = "";
			uint8_t c0 = _display->seg1[y][x], c1 = _display->seg1[y+1][x];
			for(int i = 0; i < 8; i++){
				int p = (((c1 >> i) & 0x01) * 2) + ((c0 >> i) & 0x01);
				switch(p){
					case 3: s += "█"; break;
					case 2: s += "▄"; break;
					case 1: s += "▀"; break;
					case 0: s += " "; break;
				}
			}
            Serial.print(s.c_str());
		}
        for(int x = 0; x < 8; x++) {
			std::string s = "";
			uint8_t c0 = _display->seg2[y][x], c1 = _display->seg2[y+1][x];
			for(int i = 0; i < 8; i++){
				int p = (((c1 >> i) & 0x01) * 2) + ((c0 >> i) & 0x01);
				switch(p){
					case 3: s += "█"; break;
					case 2: s += "▄"; break;
					case 1: s += "▀"; break;
					case 0: s += " "; break;
				}
			}
            Serial.print(s.c_str());
		}
        Serial.println();
	}
}

void setup(void) {
    Serial.begin(9600); delay(400);
    Serial.println("HELLO WORLD! ");
    display::init();
    Serial.println();
}

int i = 0;

void loop() {
    display::clearBuffer();
	display::renderCar(0.1 * i);
    display::swapBuffers();

    display::debugDrawToSerial();
	i++;
	delay(300);
}