#include "display/Fonts.h"
#include "display/display.h"
#include "dashstorage.h"
#include "input/button.h"
#include <Time.h>
#include <FlexCAN_T4.h>

//FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

#define PIN_RTD 24


void buttonFuncRTD();
void buttonFuncLap();

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

    button::addButton( PIN_RTD, &buttonFuncRTD, nullptr, true);
    button::addButton(      11, &buttonFuncLap, nullptr, false);

    pinMode(16, 1);
    pinMode(17, 0);
    pinMode(18, 0);
    pinMode(19, 1);
}

int frameNumber = 0;

bool RTD = false;

uint32_t    buttonCheckTime = 0;

uint32_t    dtime = 200;
uint32_t    lapstart = 0;
int32_t     lapdelta = 0;
uint32_t    lastlap = 0;
char        timestr[9];

void buttonFuncRTD(){
    RTD = !RTD;
    digitalWrite(0,  RTD);
    digitalWrite(1, !RTD);
}

void buttonFuncLap(){
    uint32_t t = millis();
    int32_t laptime = t - lapstart;
    lapdelta = laptime - lastlap;
    lapstart = t;
    lastlap = laptime;
}

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
    if (millis() > dtime) {
        display::clearBuffer();
	
        if(!RTD) {
            display::renderCar(0.1 * frameNumber);
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

        Serial.print(millis() - dtime);
        Serial.print(' ');
        display::swapBuffers();
        Serial.println(millis() - dtime);
        dtime = millis() + 150;
        frameNumber++;
    }

    if(millis() >= buttonCheckTime) { //fixes jitter
        button::updateButtons();
        buttonCheckTime = millis() + 20;
    }
}
