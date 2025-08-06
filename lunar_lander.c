#include "lunar_lander.h"

#include <time.h>
#include <assert.h>

static LunarLander lunar_lander;

void lunar_lander_init(int screen_width, int screen_height)
{
  SetRandomSeed((unsigned int)time(NULL));

  float x = 0;
  float start_y = screen_height - LINE_START_Y;
  const float delta_x = (float)screen_width / LINE_NUM;

  lunar_lander.lander_start_index = GetRandomValue(0, LINE_NUM / 2);
  lunar_lander.lander_end_index = lunar_lander.lander_start_index + LANDER_LINE_NUM;
  assert(lunar_lander.lander_end_index < LINE_NUM);

  lunar_lander.lander_goal_start_index = GetRandomValue((LINE_NUM / 2) + 1, LINE_NUM - LANDER_LINE_NUM - 1);
  lunar_lander.lander_goal_end_index = lunar_lander.lander_goal_start_index + LANDER_LINE_NUM;
  assert(lunar_lander.lander_goal_end_index < LINE_NUM);

  for (int i = 0; i < LINE_NUM; i++)
  {
    float last_y = start_y;
    float random_offset = GetRandomValue(-LINE_OFFSET_Y, LINE_OFFSET_Y);
    if (i != 0)
    {
      last_y = lunar_lander.lines[i - 1].end.y;
    }

    bool plotting_start = i >= lunar_lander.lander_start_index && i < lunar_lander.lander_end_index;
    bool plotting_goal = i >= lunar_lander.lander_goal_start_index && i < lunar_lander.lander_goal_end_index;
    if (plotting_start || plotting_goal)
    {
      random_offset = 0;
    }

    lunar_lander.lines[i] = (Line){
        .start = (Vector2){
            x,
            last_y,
        },
        .end = (Vector2){
            x + delta_x,
            start_y + random_offset,
        }};

    x += delta_x; // Step x forward
  }
}

void lunar_lander_draw_map()
{
  for (int i = 0; i < LINE_NUM; i++)
  {
    Color color = WHITE;
    bool plotting_start = i > lunar_lander.lander_start_index && i < lunar_lander.lander_end_index;
    bool plotting_goal = i > lunar_lander.lander_goal_start_index && i < lunar_lander.lander_goal_end_index;
    if (plotting_start)
    {
      color = GREEN;
    }
    else if (plotting_goal)
    {
      color = BLUE;
    }
    // DrawCircle(lunar_lander.lines[i].start.x, lunar_lander.lines[i].start.y, 5, RED);
    DrawLine(lunar_lander.lines[i].start.x, lunar_lander.lines[i].start.y, lunar_lander.lines[i].end.x, lunar_lander.lines[i].end.y, color);
  }
}