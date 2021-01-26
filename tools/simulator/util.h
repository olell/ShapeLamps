#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>

uint32_t hsv_to_rgb(float h, float s, float v){
    if(h > 360 || h < 0 || s > 100 || s < 0 || v > 100 || v < 0){
        return 0x00000000;
    }
    s = s / 100.0;
    v = v / 100.0;
    float c = s*v;
    float x = c*(1-abs(fmod(h/60.0, 2)-1));
    float m = v-c;
    float r,g,b;
    if(h >= 0 && h < 60){
        r = c,g = x,b = 0;
    }
    else if(h >= 60 && h < 120){
        r = x,g = c,b = 0;
    }
    else if(h >= 120 && h < 180){
        r = 0,g = c,b = x;
    }
    else if(h >= 180 && h < 240){
        r = 0,g = x,b = c;
    }
    else if(h >= 240 && h < 300){
        r = x,g = 0,b = c;
    }
    else{
        r = c,g = 0,b = x;
    }
    uint8_t br = (r+m)*255;
    uint8_t bg = (g+m)*255;
    uint8_t bb = (b+m)*255;
    
    uint32_t col = 0x00000000;
    col |= br;
    col |= bg << 8;
    col |= bb << 16;

    return col;
}