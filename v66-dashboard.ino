#include "display/Fonts.h"
#include "display/display.h"
#include "dashstorage.h"
#include <Time.h>
#include <FlexCAN_T4.h>

//FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

#define PIN_RTD 24


void setup(void) {
    Serial.begin(9600); delay(400);
    Serial.println("HELLO WORLD! ");
    delay(10);
    display::init();
    Serial.println();

    pinMode(0, 1);
    pinMode(1, 1);
    digitalWrite(0, 0);
    digitalWrite(1, 0);

    pinMode(11, INPUT);
    digitalWrite(11, HIGH);
    pinMode(12, INPUT);
    digitalWrite(12, HIGH);
    pinMode(PIN_RTD, INPUT);
    digitalWrite(PIN_RTD, HIGH);

    pinMode(0, 1);
    pinMode(1, 1);
    digitalWrite(1, 1);

    pinMode(16, 1);
    pinMode(17, 1);
    pinMode(18, 1);
    pinMode(19, 1);
}

int frameNumber = 0;

bool RTD = false;
bool rtdbuffer = true;
bool pin12buffer = true;


uint32_t    dtime = 200;
uint32_t    lapstart = 0;
int32_t     lapdelta = 0;
uint32_t    lastlap = 0;
char        timestr[9];

void getTimeStrFrom(int32_t ms, bool addsign) {
    int cen = (abs(ms) % 1000) / 10;
    int sec = (abs(ms) / 1000) % 60;
    int min =((abs(ms) / 1000) % 3600) / 60;

    timestr[0] = '0' + min / 10;
    timestr[1] = '0' + min % 10;

    timestr[2] = ':';

    timestr[3] = '0' + sec / 10;
    timestr[4] = '0' + sec % 10;

    timestr[5] = ':';

    timestr[6] = '0' + cen / 10;
    timestr[7] = '0' + cen % 10;

    timestr[8] = '\0';

    if(addsign) timestr[0] = (ms > 0) ? '+' : '-';
}

void loop() {

    if(!digitalRead(PIN_RTD) && rtdbuffer){
        RTD = !RTD;
        digitalWrite(0,  RTD);
        digitalWrite(1, !RTD);
        rtdbuffer = false;
    }
    else if(digitalRead(PIN_RTD))
        rtdbuffer = true;
    if(digitalRead(12) && pin12buffer){
        uint32_t t = millis();
        int32_t laptime = t - lapstart;
        lapdelta = laptime - lastlap;
        lapstart = t;
        lastlap = laptime;
        pin12buffer = false;
    }
    else if(!digitalRead(12))
        pin12buffer = true;
    
    if (millis() > dtime) {
        display::clearBuffer();
	
        if(!RTD) {
            display::renderCar(0.2 * frameNumber);
            display::drawText(  0,  0,  0, "ROTATES YOUR CAR");
            display::drawText( 23, 55,  0, "BOTTOM TEXT");
        }
        else {
            display::drawText(  4,  0,  0, "LAPTIME");
            getTimeStrFrom(dtime - lapstart, 0);
            display::drawText(  6, 12,  1, timestr);
            getTimeStrFrom(lapdelta, 1);
            display::drawText(  6, 21,  1, timestr);
            display::drawLine( 62,  0, 62, 30);
            display::drawLine(  0, 30, 62, 30);
            display::drawText( 70,  0,  0, "LAPS ON");
            display::drawText( 70,  9,  0, "BATTERY");
        }

        display::swapBuffers();
        dtime = millis() + 100;
        frameNumber++;
    }

    delay(10); //fixes jitter in the buttons
}
