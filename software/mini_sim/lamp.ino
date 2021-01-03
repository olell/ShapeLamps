uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void update_leds() {
  uint8_t led_x;
  uint8_t led_y;

  float r, g, b, a;

  for (int i = 0; i < LED_COUNT; i ++) {

    led_x = map((led_map[i] >> 8) & 0xff, 0, 0xff, 20, 220);
    led_y = map(led_map[i] & 0xff, 0, 0xff, 20, 220);

    r = colors[i] & 0xff;
    g = (colors[i] >> 8) & 0xff;
    b = (colors[i] >> 16) & 0xff;

    if (normalize) {
        uint16_t rgbsum = r + g + b;
        uint16_t normbright = brightness * 3;
        if (rgbsum > normbright) {
            float divider = rgbsum / normbright;
            r /= divider;
            g /= divider;
            b /= divider;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
        }
    }
    else {
        r = r * (brightness / float(0xff));
        g = g * (brightness / float(0xff));
        b = b * (brightness / float(0xff));
    }

    a = (colors[i] >> 24) & 0xff; // May be used for rgbw leds in future?

    tft.fillCircle(led_x, led_y, 3, rgb565((uint8_t)r, (uint8_t)g, (uint8_t)b));

  }
}
uint32_t hsv_to_rgb(float h, float s, float v) {
    float r = 0; 
    float g = 0; 
    float b = 0;

    float hf = h / 60.0;

    int i = (int) floor(h / 60.0);
    float f = h/60.0 - i;
    float pv = v * (1 - s);
    float qv = v * (1 - s*f);
    float tv = v * (1 - s * (1 - f));

    switch (i) {
        case 0:
            r = v;
            g = tv;
            b = pv;
            break;
        case 1:
            r = qv;
            g = v;
            b = pv;
            break;
        case 2: 
            r = pv;
            g = v;
            b = tv;
            break;
        case 3:
            r = pv;
            g = qv;
            b = v;
            break;
        case 4:
            r = tv;
            g = pv;
            b = v;
            break;
        case 5:
            r = v;
            g = pv;
            b = qv;
            break;
    }

    //set each component to a integer value between 0 and 255
    uint8_t red = constrain((int) 255 * r, 0, 255);
    uint8_t green = constrain((int) 255 * g, 0, 255);
    uint8_t blue = constrain((int) 255 * b, 0, 255);

    return (blue << 16 | green << 8 | red);

}

void set_segment(uint8_t x, uint8_t y, uint32_t color) {
    uint8_t l;
    for (int i = 0; i < LED_GRID_PER_CELL; i ++){
        l = led_grid[y][x][i];
        if (l >= 0) {
            colors[l] = color;
        }
    }
}

void clear() {
    for (int i = 0; i < LED_COUNT; i ++) colors[i] = 0;
}