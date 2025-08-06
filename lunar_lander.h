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

/// T - triangle, L - leg
typedef enum
{
  T0 = 0,
  T1,
  T2,
  L0,
  L1,
  L2,
  L3,
  NUM_POINTS
} LanderPoints;

// Struct for keeping state of the Lunar Lander game
typedef struct LunarLander
{
  Line lines[LINE_NUM];
  int lander_start_index;
  int lander_end_index;

  int lander_goal_start_index;
  int lander_goal_end_index;

  // Body definition
  Vector2 lander_pos;
  Vector2 lander_body[NUM_POINTS];

  // Physics
  Vector2 lander_force;
  Vector2 lander_vel;
  float lander_mass;
} LunarLander;

static Vector2 xy(float x, float y)
{
  Vector2 vec2;
  vec2.x = x;
  vec2.y = y;
  return vec2;
}

void lunar_lander_init(int screen_width, int screen_height);
void lunar_lander_update(float dt);
void lunar_lander_render();
void lander_apply_force(Vector2 force);