#include "raylib/raylib.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib/clay_renderer_raylib.c"

// 16:9 resolution
const float ScreenWidth = 1408.0f;
const float ScreenHeight = 792.0f;

const Clay_Color COLOR_LIGHT = (Clay_Color){224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color){168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color){225, 138, 50, 255};
const Clay_Color COLOR_WHITE = (Clay_Color){.r = 255, .g = 255, .b = 255};
const Clay_Color COLOR_DARK = (Clay_Color){100, 100, 100, 255};

Clay_RenderCommandArray BuildLayout(void)
{
  // Create layout
  Clay_BeginLayout();

  CLAY(CLAY_ID("Main"), CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16}), CLAY_RECTANGLE({.color = COLOR_DARK}))
  {
    CLAY(CLAY_ID("Header"),
         CLAY_LAYOUT({.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(50)}, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .childGap = 16, .padding = {32}}),
         CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    {
    }

    CLAY(CLAY_ID("CenterContainer"), CLAY_LAYOUT({.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .childGap = 16}), CLAY_RECTANGLE({.color = COLOR_DARK}))
    {

      CLAY(CLAY_ID("SideBar1"),
           CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16}),
           CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      {
      }
      CLAY(CLAY_ID("MainContent"),
           CLAY_LAYOUT({.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}),
           CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      {
      }

      CLAY(CLAY_ID("SideBar2"),
           CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16}),
           CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      {
      }
    }

    CLAY(CLAY_ID("Footer"),
         CLAY_LAYOUT({.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(200)}, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .childGap = 16, .padding = {32}}),
         CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    {
    }
  }

  return Clay_EndLayout();
}

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
  Clay_SetMeasureTextFunction(Raylib_MeasureText);
  Clay_Initialize(memoryArena, dimensions);

  // ...more init??
  SetTargetFPS(144);

  // Main application loop
  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();

    // Logic code

    // Update states
    Vector2 raylibMousePosition = GetMousePosition();
    Clay_Vector2 mousePosition = {
        .x = raylibMousePosition.x,
        .y = raylibMousePosition.y,
    };
    Clay_SetPointerState(mousePosition, IsMouseButtonDown(MOUSE_LEFT_BUTTON));

    Vector2 raylibMouseWheelDelta = GetMouseWheelMoveV();
    Clay_Vector2 mouseWheelDelta = {
        .x = raylibMouseWheelDelta.x,
        .y = raylibMouseWheelDelta.y,
    };
    Clay_UpdateScrollContainers(true, mouseWheelDelta, dt);

    Clay_RenderCommandArray renderCommands = BuildLayout();

    // Render stuff
    BeginDrawing();
    ClearBackground(YELLOW);
    // TODO: render the Clay layout
    Clay_Raylib_Render(renderCommands);
    EndDrawing();
  }

  return 0;
}