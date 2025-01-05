#include "raylib/raylib.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib/clay_renderer_raylib.c"

// =================================
//              COLORS
// =================================
const Clay_Color COLOR_LIGHT = (Clay_Color){224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color){168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color){225, 138, 50, 255};
const Clay_Color COLOR_WHITE = (Clay_Color){.r = 255, .g = 255, .b = 255};
const Clay_Color COLOR_DARK = (Clay_Color){100, 100, 100, 255};
const Clay_Color COLOR_BUTTON = (Clay_Color){20, 20, 20, 255};
const Clay_Color COLOR_BUTTON_HOVER = (Clay_Color){40, 40, 40, 255};

// =================================
//            FONT IDS
// =================================
const uint32_t FONT_ID_NORMAL_16 = 0;
const uint32_t FONT_ID_NORMAL_24 = 1;
const uint32_t FONT_ID_BOLD_16 = 2;
const uint32_t FONT_ID_BOLD_24 = 3;
const uint32_t FONT_ID_ITALIC_16 = 4;
const uint32_t FONT_ID_ITALIC_24 = 5;
const uint32_t FONT_ID_TITLE = 6;

// =================================
//      ELEMENT CONFIGURATIONS
// =================================
const Clay_TextElementConfig headerTextConfig = (Clay_TextElementConfig){.fontId = FONT_ID_TITLE, .fontSize = 36, .textColor = COLOR_DARK};
const Clay_TextElementConfig footerTextConfig = (Clay_TextElementConfig){.fontId = FONT_ID_BOLD_16, .fontSize = 16, .textColor = COLOR_DARK};
const Clay_TextElementConfig buttonTextConfig = (Clay_TextElementConfig){.fontId = FONT_ID_NORMAL_24, .fontSize = 24, .textColor = COLOR_LIGHT};

Clay_LayoutConfig buttonLayoutConfig = {
    .sizing = {
        .width = CLAY_SIZING_GROW(),
        .height = CLAY_SIZING_FIXED(50),
    },
    .childAlignment = {.y = CLAY_ALIGN_Y_CENTER, .x = CLAY_ALIGN_X_CENTER},
};

// 16:9 resolution
const float ScreenWidth = 1400.0f; // UPDATED
const float ScreenHeight = 850.0f; // UPDATED

void handle_button_interaction(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData)
{
}

void button_element(const char *id, Clay_String text)
{
  CLAY(CLAY_ID(id),
       CLAY_LAYOUT(buttonLayoutConfig),
       CLAY_RECTANGLE({.color = Clay_Hovered() ? COLOR_BUTTON_HOVER : COLOR_BUTTON}))
  {
    CLAY_TEXT(text, CLAY_TEXT_CONFIG(buttonTextConfig));
  }
}

Clay_RenderCommandArray build_layout(void)
{
  // Create layout
  Clay_BeginLayout();

  CLAY(CLAY_ID("Main"), CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16}), CLAY_RECTANGLE({.color = COLOR_DARK}))
  {
    CLAY(CLAY_ID("Header"),
         CLAY_LAYOUT({.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(50)}, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}, .childGap = 16, .padding = {32}}),
         CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    {
      CLAY_TEXT(CLAY_STRING("Mars Rover Control Panel"), CLAY_TEXT_CONFIG(headerTextConfig));
    }

    CLAY(CLAY_ID("CenterContainer"), CLAY_LAYOUT({.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .childGap = 16}), CLAY_RECTANGLE({.color = COLOR_DARK}))
    {
      CLAY(CLAY_ID("SideBar1"),
           CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16}),
           CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      {
        // When id's are button_1, button_2, button_3, button_4, they all seem to get the same hash?
        // Should investigate the exact output of the clay hashing function
        // assert(Clay__HashString(CLAY_STRING("button_1"), 0, 0) == Clay__HashString(CLAY_STRING("button_1"), 0, 0)); ??????

        button_element("button_1", CLAY_STRING("Power On"));
        button_element("button_2", CLAY_STRING("Power Thrusters"));
        button_element("button_3", CLAY_STRING("Collect Samples"));
        button_element("button_4", CLAY_STRING("Emergency Shutdown"));
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
         CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(200)}, .childAlignment = {0, 0}, .childGap = 16, .padding = {32}}),
         CLAY_RECTANGLE({.color = COLOR_LIGHT, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    {
      CLAY_TEXT(CLAY_STRING(""), CLAY_TEXT_CONFIG(footerTextConfig));
      CLAY_TEXT(CLAY_STRING("> Samples Collected by Rover"), CLAY_TEXT_CONFIG(footerTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power Thrusters"), CLAY_TEXT_CONFIG(footerTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power On"), CLAY_TEXT_CONFIG(footerTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power Off"), CLAY_TEXT_CONFIG(footerTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power On"), CLAY_TEXT_CONFIG(footerTextConfig));
    }
  }

  return Clay_EndLayout();
}

void insert_font(uint32_t font_id, int font_size, const char *path)
{
  Raylib_fonts[font_id].fontId = font_id;
  Raylib_fonts[font_id].font = LoadFontEx(path, font_size * 2, NULL, 0);
  SetTextureFilter(Raylib_fonts[font_id].font.texture, TEXTURE_FILTER_TRILINEAR);
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

  insert_font(FONT_ID_NORMAL_16, 16, "resources/0xProto-Regular.ttf");
  insert_font(FONT_ID_NORMAL_24, 24, "resources/0xProto-Regular.ttf");
  insert_font(FONT_ID_BOLD_16, 16, "resources/0xProto-Bold.ttf");
  insert_font(FONT_ID_BOLD_24, 24, "resources/0xProto-Bold.ttf");
  insert_font(FONT_ID_ITALIC_16, 16, "resources/0xProto-Italic.ttf");
  insert_font(FONT_ID_ITALIC_24, 24, "resources/0xProto-Italic.ttf");
  insert_font(FONT_ID_TITLE, 36, "resources/0xProto-Regular.ttf");

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

    Clay_RenderCommandArray renderCommands = build_layout();

    // Render stuff
    BeginDrawing();
    ClearBackground(YELLOW);
    // TODO: render the Clay layout
    Clay_Raylib_Render(renderCommands);
    EndDrawing();
  }

  return 0;
}