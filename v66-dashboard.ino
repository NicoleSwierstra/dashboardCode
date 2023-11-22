#include "display/Fonts.h"
#include "display/display.h"
#include "dashstorage.h"
#include "input/button.h"
#include <Time.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

#define PIN_LED_LC          0x10
#define PIN_LED_FAULT       0x11
#define PIN_RTD_LIGHT_B     0x0F
#define PIN_RTD_LIGHT_G     0x12
#define PIN_RTD_LIGHT_R     0x13

#define PIN_AUDIO_LRCLK1    0x14
#define PIN_AUDIO_BCLK1     0x15
#define PIN_AUDIO_OUT1B     0x20

#define PIN_CAN_RX          0x16
#define PIN_CAN_TX          0x17

#define PIN_LCD_DIMMER_POT  0x18
#define PIN_LED_DIMMER_PWM  0x0E
#define PIN_LCD_DIMMER_PWM  0x21
#define PIN_LED_DIMMER_POT  0x1A

#define PIN_BUTTON_N0       0x19
#define PIN_BUTTON_N1       0x1B
#define PIN_BUTTON_N2       0x1D
#define PIN_BUTTON_PAGE     0x1F

#define PIN_IMD_MONITOR     0x1C
#define PIN_AMS_MONITOR     0x1E

void buttonFuncRTD();
void buttonFuncLap();

void setup(void) {
    Serial.begin(9600); delay(400);
    delay(10);
    display::init();
    Serial.println();

    pinMode(PIN_BUTTON_PAGE, OUTPUT);

    button::addButton( PIN_BUTTON_N0, &buttonFuncRTD, nullptr, true);
    button::addButton( PIN_BUTTON_N1, &buttonFuncLap, nullptr, false);
}

int frameNumber = 0;

bool RTD = false;

uint32_t    buttonCheckTime = 0;

uint32_t    dtime = 200;
uint32_t    lapstart = 0;
int32_t     lapdelta = 0;
uint32_t    lastlap = 0;

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

char* getTimeStrFrom(int32_t ms, bool addsign) {
    static char timestr[9];

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
    return timestr;
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
            display::drawText(  6, 12,  1, getTimeStrFrom(dtime - lapstart, 0));
            display::drawText(  6, 21,  1, getTimeStrFrom(lapdelta, 1));
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
