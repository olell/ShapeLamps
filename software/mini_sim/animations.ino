#define OFF_ANIMATION           0
#define RAINBOW_ANIMATION       1
#define RAINBOW_CYCLE_ANIMATION 2
#define SINGLE_COLOR            3
#define RAINBOW_BARS            4
#define AUDIO_VISUAL            5

uint8_t running_animation = OFF_ANIMATION;

// rainbow animation
uint16_t rainbow_anim_hue = 0;

void start_animation(const char* name) {
    clear();
    if (name == "off") {
        running_animation = OFF_ANIMATION;
    }
    else if (name == "rainbow") {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_ANIMATION;
    }
    else if (name == "rainbow_cycle") {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_CYCLE_ANIMATION;
    }
    else if (name == "single_color") {
        running_animation = SINGLE_COLOR;
    }
    else if (name == "rainbow_bars") {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_BARS;
    }
    else if (name == "audio_visual") {
        running_animation = AUDIO_VISUAL;
    }
}

void run_animations() {
    if (running_animation == OFF_ANIMATION) {
    }

    if (running_animation == RAINBOW_ANIMATION) {
        rainbow_animation();
    }

    if (running_animation == RAINBOW_CYCLE_ANIMATION) {
        rainbow_cycle_animation();
    }

    if (running_animation == SINGLE_COLOR) {
        single_color_animation();
    }

    if (running_animation == RAINBOW_BARS) {
        rainbow_bars_animation();
    }

    if (running_animation == AUDIO_VISUAL) {
        audio_visual_animation();
    }
}

void rainbow_animation() {
    uint32_t col = hsv_to_rgb(((float)rainbow_anim_hue / 1000.0) * 360.0, 1, 1);
    for (int i = 0; i < LED_COUNT; i++) {
        colors[i] = col;
    }
    rainbow_anim_hue ++;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void rainbow_cycle_animation() {
    uint32_t col;
    float hue;
    for (int i = RING_FIRST; i <= RING_LAST; i ++) {
        hue = ((float)rainbow_anim_hue / 1000.0) + ((float) (i - RING_FIRST) / float(RING_LAST - RING_FIRST));
        if (hue >= 1) hue -= 1;
        col = hsv_to_rgb(hue * 360, 1, 1);
        colors[i] = col;

        if (i == 46) {
            for (int j = SMALL_FLAG_FIRST; j <= SMALL_FLAG_LAST; j++) {
                colors[j] = col;
            }
        }
        if (i == 75) {
            for (int j = BIG_FLAG_FIRST; j <= BIG_FLAG_LAST; j++) {
                colors[j] = col;
            }
        }
    }
    rainbow_anim_hue ++;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void single_color_animation() {
    if (single_color_type == 0) {
        for (int i = 0; i < LED_COUNT; i ++){
            colors[i] = single_color_all;
        }
    }
    if (single_color_type == 1) {
        for (int i = RING_FIRST; i <= RING_LAST; i ++)
            colors[i] = ring_color;
        for (int i = SMALL_FLAG_FIRST; i <= SMALL_FLAG_LAST; i ++)
            colors[i] = small_flag_color;
        for (int i = BIG_FLAG_FIRST; i <= BIG_FLAG_LAST; i ++)
            colors[i] = big_flag_color;
    }
}

void rainbow_bars_animation() {
    uint32_t col;
    float hue;
    for (int y = 0; y < LED_GRID_HEIGHT; y ++) {
        hue = ((float)rainbow_anim_hue / 1000.0) + 2*((float) y / (float) LED_GRID_HEIGHT);
        while (hue > 1) hue -= 1;
        col = hsv_to_rgb(hue * 360, 1, 1);
        for (int x = 0; x < LED_GRID_WIDTH; x ++) {
            set_segment(x, y, col);
        }
    }
    rainbow_anim_hue ++;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

#define SAMPLES 256
#define SAMPLING_FREQUENCY 20000
unsigned int sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
unsigned long microseconds;
byte peak[] = {0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int dominant_value;

int min_dsize = 1000;
int dmax = 127;
int ring_count = RING_LAST - RING_FIRST;
int per_band = ring_count / 8;

float ring_offset = 0;

void audio_visual_animation() {
    for (int i = 0; i < SAMPLES; i ++) {
        newTime = micros();
        vReal[i] = analogRead(AUDIO_PIN);
        vImag[i] = 0;
        while ((micros() - newTime) < sampling_period_us) {}
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    displayBand(0,(int)vReal[2]); // 125Hz
    displayBand(1,(int)vReal[4]); // 250Hz
    displayBand(2,(int)vReal[7]); // 500Hz
    displayBand(3,(int)vReal[15]); // 1000Hz
    displayBand(4,(int)vReal[40]); // 2000Hz
    displayBand(5,(int)vReal[70]); // 4000Hz
    displayBand(6,(int)vReal[288]); // 8000Hz
    displayBand(7,(int)vReal[(SAMPLES/2) - 1]); // 16000Hz
}

uint32_t col;

void displayBand(int band, int dsize) {

    if (dsize < min_dsize) min_dsize = dsize;
    dsize -= min_dsize;
    if (dsize < 0) dsize = 0;

    dsize /= VA_MAX_READ;
    if (dsize > dmax) dsize = dmax;
    
    col = hsv_to_rgb(0, 0, dsize / (float) dmax);

    for (int i =
        
        RING_FIRST + ((band * per_band) + (int) ring_offset) % ring_count; 
        i < RING_FIRST + (((band + 1) * per_band) + (int) ring_offset) % ring_count; 
        
        i++){
        colors[i] = col;
    }
    //Serial.printf("%02d -> %5d\r\n", band, dsize);

    if (ring_offset > ring_count) ring_offset = 0;
}