#include "display.h"
#include <stdlib.h>
#include <string.h>
#include "Fonts.h"
#include "VehicleWireframeData.h"
#include <math.h>
#include <Arduino.h>

display::buffer *display::_active, *display::_display;

void display::setPixel(int x, int y) {
	if(x < 0 || x > WIDTH || y < 0 || y > HEIGHT) return;

    (x < WIDTH/2) ? 
		_active->seg1[y / 8][x]				|= 0x1 << (y % 8) : 
		_active->seg2[y / 8][x - (WIDTH/2)] |= 0x1 << (y % 8);
}

void writePortD(uint8_t n) {
	digitalWrite(  2, (n >> 0) & 0x1);
	digitalWrite( 14, (n >> 1) & 0x1);
	digitalWrite(  7, (n >> 2) & 0x1);
	digitalWrite(  8, (n >> 3) & 0x1);
	digitalWrite(  6, (n >> 4) & 0x1);
	digitalWrite( 20, (n >> 5) & 0x1);
	digitalWrite( 21, (n >> 6) & 0x1);
	digitalWrite(  5, (n >> 7) & 0x1);
}

uint8_t readPortD() {
	return
		(digitalRead(  2) << 0) +
		(digitalRead( 14) << 1) +
		(digitalRead(  7) << 2) +
		(digitalRead(  8) << 3) +
		(digitalRead(  6) << 4) +
		(digitalRead( 20) << 5) +
		(digitalRead( 21) << 6) +
		(digitalRead(  5) << 7);
}

void portDpinMode(uint8_t m){
	pinMode(  2, m);
	pinMode( 14, m);
	pinMode(  7, m);
	pinMode(  8, m);
	pinMode(  6, m);
	pinMode( 20, m);
	pinMode( 21, m);
	pinMode(  5, m);
}

void flashEnable(){
	digitalWrite(DISPLAY_PIN_ENABLE, 	1);
	delayMicroseconds(1);
	digitalWrite(DISPLAY_PIN_ENABLE, 	0);
}

void waitUntilNotBusy(int n){
	portDpinMode(INPUT);

	digitalWrite(DISPLAY_PIN_RS, 0);
	digitalWrite(DISPLAY_PIN_RW, 1);

	bool isBusy = true;
	int i = 0;
	while(isBusy && i < n){
		digitalWrite(DISPLAY_PIN_ENABLE,	1);

		delayMicroseconds(1);
		isBusy = digitalRead(5);

		digitalWrite(DISPLAY_PIN_ENABLE,	0);
		delayNanoseconds(100);
		i++;
	}
	portDpinMode(OUTPUT);
}

void printStatus() {
	portDpinMode(INPUT);

	digitalWrite(DISPLAY_PIN_RS, 0);
	digitalWrite(DISPLAY_PIN_RW, 1);
	digitalWrite(DISPLAY_PIN_ENABLE,	1);

	delayMicroseconds(1);

	Serial.print("Status Check: ");
	Serial.println(readPortD(), BIN);

	digitalWrite(DISPLAY_PIN_ENABLE,	0);

	portDpinMode(OUTPUT);
}

void display::init(){
	_active = new buffer();
	_display = new buffer();

	pinMode(DISPLAY_PIN_RS,		1);
	pinMode(DISPLAY_PIN_RW,		1);
	pinMode(DISPLAY_PIN_ENABLE,	1);
	pinMode(DISPLAY_PIN_RESET,	1);
	pinMode(DISPLAY_PIN_CS1,	1);
	pinMode(DISPLAY_PIN_CS2,	1);

	portDpinMode(OUTPUT);

	writePortD(0b00000000);

	digitalWrite(DISPLAY_PIN_CS1, 		1);
	digitalWrite(DISPLAY_PIN_CS2, 		1);

	digitalWrite(DISPLAY_PIN_RESET, 	1);
	flashEnable();
	
	waitUntilNotBusy(0);
	delay(1);
	digitalWrite(DISPLAY_PIN_RS, 		0);
	digitalWrite(DISPLAY_PIN_RW, 		0);
	writePortD(0b00111111);
	flashEnable();

	printStatus();
}

void display::swapBuffers(){
	buffer* tmp = _display;
	_display = _active;
	_active = tmp;

	//printStatus();

	digitalWrite(DISPLAY_PIN_RS, 		0);
	digitalWrite(DISPLAY_PIN_RW, 		0);
	digitalWrite(DISPLAY_PIN_ENABLE, 	0);
	digitalWrite(DISPLAY_PIN_CS2, 		1);
	digitalWrite(DISPLAY_PIN_CS1, 		1);
	delayMicroseconds(5);
	writePortD(0b01000000);
	flashEnable();
	delayMicroseconds(1);

	digitalWrite(DISPLAY_PIN_RS, 		1);
	
	for(int y = 0; y < 8; y++) {
		delayMicroseconds(8);

		//Select page of display
		digitalWrite(DISPLAY_PIN_RS, 0);
		writePortD(0b10111000 | y);
		flashEnable();
		digitalWrite(DISPLAY_PIN_RS, 1);
		delayMicroseconds(1);

		for(int x = 0; x < WIDTH / 2; x++) {
			digitalWrite(DISPLAY_PIN_CS2, 0);
			delayMicroseconds(3);

			writePortD(_display->seg1[y][x]);
			flashEnable();

			digitalWrite(DISPLAY_PIN_CS1, 0);
			digitalWrite(DISPLAY_PIN_CS2, 1);
			delayMicroseconds(3);

			writePortD(_display->seg2[y][x]);
			flashEnable();
			digitalWrite(DISPLAY_PIN_CS1, 1);
		}
	}
}

void display::clearBuffer(){
	for(int line = 0; line < HEIGHT/8; line++){
		for(uint8_t& block : _active->seg1[line]){
			block = 0x00;
		}
		for(uint8_t& block : _active->seg2[line]){
			block = 0x00;
		}
	}
}

//this is someone elses code. Probably dont read it
void display::drawLine(int x, int y, int x2, int y2) {
	bool yLonger = false;
	int incrementVal, endVal;
	int shortLen = y2 - y;
	int longLen = x2 - x;

	if (abs(shortLen) > abs(longLen)) {
		int swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	
	endVal = longLen;
	
	if (longLen < 0) {
		incrementVal = -1;
		longLen = -longLen;
	} 
	else 
		incrementVal = 1;
	
	int decInc;
	if (longLen == 0) 
		decInc = 0;
	else 
		decInc = (shortLen << 16) / longLen;
	
	int j = 0;
	if (yLonger) {	
		for (int i = 0; i != endVal; i += incrementVal) {
			setPixel(x + (j >> 16), y + i);	
			j += decInc;
		}
	} 
	else {
		for (int i=0;i!=endVal;i+=incrementVal) {
			setPixel(x + i, y + (j >> 16));
			j += decInc;
		}
	}
}

void display::drawText(int xoff, int yoff, int font, const char* text){
	int len = strlen(text);
	switch (font){
		case 0:
		for(int l = 0; l < 8; l++) {
			for(int cp = 0; cp < len; cp++){ // character pointer
				uint8_t c = fonts::text6by8ND[(int)text[cp]][l];
				for(int x = 0; x < 8; x++) {
					if((c >> (7 - x)) & 0x01) 
						setPixel((cp * 8) + x + xoff, yoff+l);
				}
			}
		} break;
		case 1:
		for(int l = 0; l < 8; l++) {
			for(int cp = 0; cp < len; cp++){ // character pointer
				uint8_t c = fonts::text5by7MC[(int)text[cp]][l];
				for(int x = 0; x < 6; x++) {
					if((c >> (7 - x)) & 0x01) 
						setPixel((cp * 6) + x + xoff, yoff+l);
				}
			}
		} break;
	}
		
}

void transformToScreen(const double * _in, double a, int * out){
    double x = _in[0], y = _in[1], z = _in[2];
    out[0] = -(int)(x * 15 * sin(a)) - (int)(z * 15 * cos(a)) + display::WIDTH/2;
    out[1] = -(int)(y *  13) + (int)(x * 3 * cos(a)) - (int)(z * 3 * sin(a)) + (display::HEIGHT/2 + 15);
}

void display::renderCar(double rot) {
	static int points[2] = {0, 0};
	static int points2[2] = {0, 0};
	for(int i = 0; i < 250; i++){
		auto pairs = VehicleWireframeData3D::vehiclepairs[i];
		transformToScreen(VehicleWireframeData3D::vehiclepoints[pairs[0] - 1], rot, points);
		transformToScreen(VehicleWireframeData3D::vehiclepoints[pairs[1] - 1], rot, points2);
		drawLine(points[0], points[1], points2[0], points2[1]);
	}
}

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