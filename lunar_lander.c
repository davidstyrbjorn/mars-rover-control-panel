#include "lunar_lander.h"
#include "raymath.h"

#include <time.h>
#include <assert.h>

static LunarLander ll;

// Size constants for the lander
#define LanderBodyWidth 30
#define LanderBodyHeight 30
#define LanderLegsHeight 10
#define LanderHeight LanderBodyHeight + LanderLegsHeight
#define LanderWidth LanderBodyWidth

#define LANDER_MASS 1.0f
#define GRAVITY_FORCE 50.0f
#define THRUST_FORCE 100.0f

void lunar_lander_init(int screen_width, int screen_height)
{
  ll.lander_pos = xy(0, 0);
  ll.lander_body[T0] = xy(0, LanderBodyHeight);
  ll.lander_body[T1] = xy(LanderBodyWidth, LanderBodyHeight);
  ll.lander_body[T2] = xy(LanderBodyWidth / 2.0, 0);
  ll.lander_body[L0] = xy(10, LanderBodyHeight);
  ll.lander_body[L1] = xy(5, LanderBodyHeight + LanderLegsHeight);
  ll.lander_body[L2] = xy(LanderBodyWidth - 10, LanderBodyHeight);
  ll.lander_body[L3] = xy(LanderBodyWidth - 5, LanderBodyHeight + LanderLegsHeight);

  ll.lander_pos = xy(100, 50); // Start somewhere on screen
  ll.lander_vel = xy(0, 0);    // Start at rest
  ll.lander_force = xy(0, 0);  // No initial force
  ll.lander_mass = LANDER_MASS;
  ll.thrust_force = xy(0, 0);

  /// Create the landscape
  SetRandomSeed((unsigned int)time(NULL));

  float x = 0;
  float start_y = screen_height - LINE_START_Y;
  const float delta_x = (float)screen_width / LINE_NUM;

  ll.lander_start_index = GetRandomValue(0, LINE_NUM / 2);
  ll.lander_end_index = ll.lander_start_index + LANDER_LINE_NUM;
  assert(ll.lander_end_index < LINE_NUM);

  ll.lander_goal_start_index = GetRandomValue((LINE_NUM / 2) + 1, LINE_NUM - LANDER_LINE_NUM - 1);
  ll.lander_goal_end_index = ll.lander_goal_start_index + LANDER_LINE_NUM;
  assert(ll.lander_goal_end_index < LINE_NUM);

  for (int i = 0; i < LINE_NUM; i++)
  {
    float last_y = start_y;
    float random_offset = GetRandomValue(-LINE_OFFSET_Y, LINE_OFFSET_Y);
    if (i != 0)
    {
      last_y = ll.lines[i - 1].end.y;
    }

    bool plotting_start = i >= ll.lander_start_index && i < ll.lander_end_index;
    bool plotting_goal = i >= ll.lander_goal_start_index && i < ll.lander_goal_end_index;
    if (plotting_start || plotting_goal)
    {
      random_offset = 0;
    }

    ll.lines[i] = (Line){
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

void lunar_lander_set_thruster_force(Vector2 thrust)
{
  ll.thrust_force = thrust;
}

static void draw_lander()
{
  // All points on the procedural lander is simply offset by the current lander position
  Vector2 lander_points[NUM_POINTS];
  for (int i = 0; i < NUM_POINTS; i++)
  {
    lander_points[i] = Vector2Add(ll.lander_body[i], ll.lander_pos);
  }
  DrawTriangleLines(lander_points[T0], lander_points[T1], lander_points[T2], WHITE);
  DrawLineV(lander_points[L0], lander_points[L1], WHITE);
  DrawLineV(lander_points[L2], lander_points[L3], WHITE);
}

void lunar_lander_update(float dt)
{
  // Apply gravity force
  ll.lander_force = xy(0, GRAVITY_FORCE);
  // Apply thruster force
  ll.lander_force = Vector2Add(ll.lander_force, ll.thrust_force);

  // Euler integration of force into a velocity, then updating position
  Vector2 acceleration = Vector2Scale(ll.lander_force, 1.0f / ll.lander_mass);
  ll.lander_vel = Vector2Add(ll.lander_vel, Vector2Scale(acceleration, dt));
  ll.lander_pos = Vector2Add(ll.lander_pos, Vector2Scale(ll.lander_vel, dt));
}

void lunar_lander_render()
{
  for (int i = 0; i < LINE_NUM; i++)
  {
    Color color = WHITE;
    bool plotting_start = i > ll.lander_start_index && i < ll.lander_end_index;
    bool plotting_goal = i > ll.lander_goal_start_index && i < ll.lander_goal_end_index;
    if (plotting_start)
    {
      color = GREEN;
    }
    else if (plotting_goal)
    {
      color = BLUE;
    }
    DrawLine(ll.lines[i].start.x, ll.lines[i].start.y, ll.lines[i].end.x, ll.lines[i].end.y, color);
  }
  draw_lander();
}