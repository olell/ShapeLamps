#include <SDL2/SDL.h> 
#include <stdio.h>

#include "map.h"
#include "util.h"

#define WINDOW_SIZE 800
#define NUM_LEDS LED_COUNT // Synonym

const float animation_speed = 127.0;

float matrix_prev[LED_GRID_HEIGHT][LED_GRID_WIDTH];
float matrix[LED_GRID_HEIGHT][LED_GRID_WIDTH];

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

SDL_Window* window;
SDL_Surface* draw_surface;
SDL_Renderer* renderer;

uint32_t leds[LED_COUNT] = { 0 };

int random(int low, int high) {
    return low + (rand() / (double) RAND_MAX) * (high - low);
}

float random_float() {
    return rand() / (float) RAND_MAX;
}

void show_leds() {
    uint8_t mx, my, r, g, b;
    uint16_t x, y;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect led_rect;
    led_rect.w = 20;
    led_rect.h = 20;
    for (int i = 0; i < LED_COUNT; i ++) {
        my = led_map[i] & 0xff;
        mx = (led_map[i] >> 8) & 0xff;

        x = ((float) mx / 0xff) * (WINDOW_SIZE - 40) + 20;
        y = ((float) my / 0xff) * (WINDOW_SIZE - 40) + 20;

        led_rect.x = x - 10;
        led_rect.y = y - 10;
        if (led_rect.x < 10) led_rect.x = 10;
        if (led_rect.y < 10) led_rect.y = 10;

        r = leds[i] & 0xff;
        g = (leds[i] >> 8) & 0xff;
        b = (leds[i] >> 16) & 0xff;

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &led_rect);

    }
    SDL_RenderPresent(renderer);
}

void set_led_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b){
    uint32_t col = 0;
    col |= r;
    col |= g << 8;
    col |= b << 16;
    leds[idx] = col;
}

void set_led_hsv(uint8_t idx, uint8_t h, uint8_t s, uint8_t v) {
    float cv_h = h / 255.0 * 360;
    float cv_s = s / 255.0 * 100;
    float cv_v = v / 255.0 * 100;
    leds[idx] = hsv_to_rgb(cv_h, cv_s, cv_v);
}

void set_segment_rgb(uint8_t idx0, uint8_t idx1, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = idx0; i < idx1; i ++)
        set_led_rgb(i, r, g, b);
}

void set_segment_hsv(uint8_t idx0, uint8_t idx1, uint8_t h, uint8_t s, uint8_t v) {
    for (int i = idx0; i < idx1; i ++)
        set_led_hsv(i, h, s, v);
}

void set_xy_rgb(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b){
    uint8_t idx;
    for (int i = 0; i < LED_GRID_PER_CELL; i ++) {
        idx = led_grid[y][x][i];
        if (idx >= 0)
            set_led_rgb(idx, r, g, b);
    }
}

void set_xy_hsv(uint8_t x, uint8_t y, uint8_t h, uint8_t s, uint8_t v){
    uint8_t idx;
    for (int i = 0; i < LED_GRID_PER_CELL; i ++) {
        idx = led_grid[y][x][i];
        if (idx >= 0)
            set_led_hsv(idx, h, s, v);
    }
}

void fire_animation () {

    int x, y;
    float heat;

    float min_cooldown = 0.01;
    float max_cooldown = 0.15;

    // 1. cooldown
    for (y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix[y][x];
            heat -= min_cooldown + random_float() * (max_cooldown - min_cooldown);
            matrix[y][x] = MAX(0, heat);
        }
    }

    // 2. raise
    float spreadsum = 0;
    for (y = LED_GRID_HEIGHT - 1; y >= 0; y --) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix[y][x] * 2;
            spreadsum = 2;
            if (y >= 1) {
                heat += matrix[y - 1][x] * 4;
                spreadsum += 4;
            }
            if (y >= 2) {
                heat += matrix[y - 2][x] * 3;
                spreadsum += 3;
            }
            if (y >= 3) {
                heat += matrix[y - 3][x] * 2;
                spreadsum += 2;
            }
            heat /= spreadsum;
            matrix[y][x] = heat;
        }
    }

    // 3. ignite
    for (y = 0; y < 4; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            if (random(0, 100) < 45) {
                matrix[y][x] += random_float() * 1.4;
                while (matrix[y][x] > 1) matrix[y][x] -= 1;
            }
        }
    }

    // "Lowpass"TM
    float prev_heat = 0;
    float lp_factor = 1;
    for (y = 0; y < LED_GRID_HEIGHT; y++) {
        for (x = 0; x < LED_GRID_WIDTH; x++) {
            heat = matrix[y][x];
            prev_heat = matrix_prev[y][x];
            lp_factor = animation_speed / 255.0;
            matrix_prev[y][x] = (1-lp_factor) * prev_heat + lp_factor * heat;
        }
    }

    // 4. display
    int h, s, v;
    for (y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix_prev[y][x];
            h = heat * 40; // Hue from read to yellowish
            s = (-heat + 0.98) * 1536;
            s = MAX(0, MIN(255, s));
            v = heat * 1024;
            v = MAX(0, MIN(255, v));
            set_xy_hsv(x, y, h, s, v);
        }
    }
}

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("AntifaLampe Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    set_segment_hsv(0, NUM_LEDS, 255, 200, 255);

    while (running) {
        while (SDL_PollEvent(&event)) { 
            switch (event.type) { 
                case SDL_QUIT: 
                    // handling of close button 
                    running = false;
                    break; 
            } 
        } 

        fire_animation();

        show_leds();

        SDL_Delay(16); // primitive 60fps fixing

    }

    return 0;
}