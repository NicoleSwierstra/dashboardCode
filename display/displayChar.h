#pragma once

//these are placeholders
#define DISPLAY_CHAR_RS 0
#define DISPLAY_CHAR_RW 1
#define DISPLAY_CHAR_EN 2

namespace displayChar {
    const int16_t CONTOUT[] = { DISPLAY_CHAR_RS, DISPLAY_CHAR_RW, DISPLAY_CHAR_EN, -1 }; //control out pins
    const int16_t DATAOUT[] = {  3,  4,  5,  6,  7,  8,  9, 10 }; //data out pins

    /**
     * @brief puts data onto the pins specified in the pins array
     * 
     * @param data - the byte of data to put onto the pins
     * @param pins - an array of length 1 to 8 that contains all pins to write to, can be terminated with -1
     */
    void putData(char data, int* pins);
    /**
     * @brief pulses the enable pin to push the pin values into the display
     */
    void pulseEnable();

    /**
     * @brief homes the cursor
     */
    void home();
    /**
     * @brief clears the display
     */
    void clear();

    /**
     * @brief inits the display
     */
    void initDisplay();

    /**
     * @brief puts a character onto the display
     * 
     * @param c char to put on the display
     */
    void putChar(char c);
    /**
     * @brief prints a string onto the display, starts at current cursor position
     * 
     * @param cstr string to put onto the display
     */
    void printString(char* cstr);
};