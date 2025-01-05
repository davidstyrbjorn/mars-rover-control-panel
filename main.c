#include "raylib/raylib.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib/clay_renderer_raylib.c"

// 16:9 resolution
const float ScreenWidth = 1408.0f;
const float ScreenHeight = 792.0f;

int main(void)
{
  // Init raylib
  Clay_Raylib_Initialize((int)ScreenWidth, (int)ScreenHeight, "Mars Rover Control Panel", FLAG_BORDERLESS_WINDOWED_MODE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

  // Init Clay
  uint64_t clayMemorySize = Clay_MinMemorySize();
  Clay_Arena memoryArena = {
      .memory = malloc(clayMemorySize),
      .capacity = clayMemorySize,
  };
  Clay_Dimensions dimensions = {
      .width = ScreenWidth,
      .height = ScreenHeight,
  };
  Clay_Initialize(memoryArena, dimensions);

  // ...more init??
  SetTargetFPS(144);

  // Main application loop
  while (!WindowShouldClose())
  {
    // Logic code

    // Render stuff
    BeginDrawing();
    ClearBackground(YELLOW);
    // TODO: render the Clay layout
    EndDrawing();
  }

  return 0;
}