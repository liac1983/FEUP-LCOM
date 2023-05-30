#pragma once

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics_const.h"
#include <math.h>

uint8_t* video_mem;

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

int (set_graphics_mode)(uint16_t mode);

int (init_graphics_mode)(uint16_t mode);

int (normalize_color)(uint32_t color, uint32_t *new_color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int deleteXpm(xpm_image_t img, uint16_t x, uint16_t y);

uint32_t (indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles);

uint32_t (direct_color)(uint32_t red, uint32_t green, uint32_t blue);

uint32_t (R_First)(uint32_t first);

uint32_t (G_First)(uint32_t first);

uint32_t (B_First)(uint32_t first);

uint32_t (R)(unsigned int h, uint8_t step, uint32_t first);

uint32_t (G)(unsigned int w, uint8_t step, uint32_t first);

uint32_t (B)(unsigned int w, unsigned int h, uint8_t step, uint32_t first);
