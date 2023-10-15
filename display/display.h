#pragma once

namespace display {
    const int WIDTH = 128, HEIGHT = 64;

    void init();

    /**
     * @brief Sets a pixel in the 
     * 
     * @param x 
     * @param y 
     * @param value 
     */
    void setPixel(int x, int y, uint8_t value, uint8_t** buffer);

    /*
    * Extremely Fast Line Algorithm Var D (Addition Fixed Point)
    * Copyright 2001, By Po-Han Lin
    *
    *
    * Freely useable in non-commercial applications as long as 
    * credits to Po-Han Lin and link to http://www.edepot.com is 
    * provided in source code and can been seen in compiled executable.  
    * Commercial applications please inquire about licensing the algorithms.
    *
    * Lastest version at http://www.edepot.com/phl.html
    */ 
    void drawLine(int x1, int y1, int x2, int y2, uint8_t** buffer);
}