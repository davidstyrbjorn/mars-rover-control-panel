#pragma once 

#include "raylib/raylib.h"

#define LINE_SECTORS_NUM 200
#define LINE_SECTOR_START_Y 250 // Pixels from the bottom
#define LINE_SECTOR_OFFSET_Y 3 // How much we randomize per sector 
#define LINE_SECTOR_MAX_OFFSET_Y 50 // Max allowed offset from LINE_SECTOR_START_Y

typedef struct LunarLander {
  Vector2 line_sectors[LINE_SECTORS_NUM];
} LunarLander;

void lunar_lander_init(int screen_width, int screen_height);
void lunar_lander_draw_line_sectors();