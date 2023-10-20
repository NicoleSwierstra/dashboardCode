#include "display.h"
#include <stdlib.h>

display::buffer *display::_active, *display::_drawing;

void display::setPixel(int x, int y, uint8_t value){
    (x < WIDTH/16) ? _active->seg1[y][x] = value : _active->seg2[y][x] = value;
}

void display::init(){
	_active = new buffer();
	_drawing = new buffer();
}

void display::swapBuffers(){
	buffer* tmp = _active;
	_active = _drawing;
	_drawing = tmp;
}

void display::clearBuffer(){
	for(int line = 0; line < HEIGHT; line++){
		for(uint8_t& block : _active->seg1[line]){
			block = 0;
		}
		for(uint8_t& block : _active->seg2[line]){
			block = 0;
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
			setPixel(x + (j >> 16), y + i, 255);	
			j += decInc;
		}
	} 
	else {
		for (int i=0;i!=endVal;i+=incrementVal) {
			setPixel(x + i, y + (j >> 16), 255);
			j += decInc;
		}
	}
}