#pragma once

#include "raylib/raylib.h"

#define LINE_NUM 60
#define LINE_START_Y 100 // Pixels from the bottom
#define LINE_OFFSET_Y 50 // How much we randomize per sector
#define LANDER_LINE_NUM 5

typedef struct Line
{
  Vector2 start;
  Vector2 end;
} Line;

typedef struct LunarLander
{
  Line lines[LINE_NUM];
  int lander_start_index;
  int lander_end_index;

  int lander_goal_start_index;
  int lander_goal_end_index;
} LunarLander;

void lunar_lander_init(int screen_width, int screen_height);
void lunar_lander_draw_map();