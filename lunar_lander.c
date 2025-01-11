#include "lundar_lander.h"

#include <time.h>

static LunarLander lunar_lander;

void lunar_lander_init(int screen_width, int screen_height)
{
  SetRandomSeed((unsigned int)time(NULL));

  float x = 0;
  float y = screen_height - LINE_SECTOR_START_Y;
  const float delta_x = screen_width / LINE_SECTORS_NUM;
  for (int i = 0; i < LINE_SECTORS_NUM; i++)
  {
    lunar_lander.line_sectors[i] = (Vector2){
        .x = x,
        .y = y,
    };
    x += delta_x;

    // Check the boundaries more precisely
    bool forced_down = y > screen_height - LINE_SECTOR_START_Y + LINE_SECTOR_MAX_OFFSET_Y;
    bool forced_up = y < screen_height - LINE_SECTOR_START_Y - LINE_SECTOR_MAX_OFFSET_Y;

    // Gradually move up or down based on random chance
    bool up = (bool)GetRandomValue(0, 1);

    // Apply the forced movement before applying the random change
    if (forced_up)
    {
      y -= LINE_SECTOR_MAX_OFFSET_Y; // Move up if forced
    }
    else if (forced_down)
    {
      y += LINE_SECTOR_MAX_OFFSET_Y; // Move down if forced
    }
    else if (up)
    {
      y -= LINE_SECTOR_MAX_OFFSET_Y; // Randomly move up
    }
    else
    {
      y += LINE_SECTOR_MAX_OFFSET_Y; // Randomly move down
    }
  }
}

void lunar_lander_draw_line_sectors()
{
  for (int i = 1; i < LINE_SECTORS_NUM; i++)
  {
    DrawLine(lunar_lander.line_sectors[i - 1].x, lunar_lander.line_sectors[i - 1].y, lunar_lander.line_sectors[i].x, lunar_lander.line_sectors[i].y, WHITE);
  }
}