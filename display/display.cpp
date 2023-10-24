#include "display.h"
#include <stdlib.h>
#include <string.h>
#include "Fonts.h"
#include "VehicleWireframeData.h"
#include <math.h>

display::buffer *display::_active, *display::_display;

void display::setPixel(int x, int y) {
    (x < WIDTH/2) ? 
		_active->seg1[y][x / 8]				|= 0x1 << (x % 8) : 
		_active->seg2[y][(x - WIDTH/2) / 8] |= 0x1 << (x % 8);
}

void display::init(){
	_active = new buffer();
	_display = new buffer();
}

void display::swapBuffers(){
	buffer* tmp = _display;
	_display = _active;
	_active = tmp;
	//TODO: code that draws the display buffer to the screen
}

void display::clearBuffer(){
	for(int line = 0; line < HEIGHT; line++){
		for(uint8_t& block : _active->seg1[line]){
			block = 0x0;
		}
		for(uint8_t& block : _active->seg2[line]){
			block = 0x0;
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
	for(int l = 0; l < 8; l++) {
		for(int cp = 0; cp < len; cp++){ // character pointer
			uint8_t c = fonts::text6by8ND[(int)text[cp]][l];
			for(int x = 0; x < 8; x++) {
				if((c >> x) & 0x01) 
					setPixel((cp * 8) + x + xoff, yoff+l);
			}
		}
	}
}

void transformToScreen(const double * _in, double a, int * out){
    double x = _in[0], y = _in[1], z = _in[2];
    out[0] = -(int)(x * 15 * sin(a)) - (int)(z * 15 * cos(a)) + display::WIDTH/2;
    out[1] = -(int)(y *  13) + (int)(x * 3 * cos(a)) - (int)(z * 3 * sin(a)) + (display::HEIGHT/2 + 10);
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