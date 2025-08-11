#include "raylib/raylib.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib/clay_renderer_raylib.c"

#include "lunar_lander.h"

Clay_LayoutElement *mainLayoutElement;
typedef struct GameWindow
{
  Rectangle window_rect;
  RenderTexture2D game_texture;
  bool has_loaded_game_texture;
} GameWindow;

GameWindow game_window = {
    .has_loaded_game_texture = false,
};

// =================================
//              COLORS
// =================================
const Clay_Color ColorLight = (Clay_Color){240 * 0.8, 231 * 0.8, 231 * 0.8, 255};
const Clay_Color ColorDark = (Clay_Color){50, 50, 50, 255};
const Clay_Color ColorButton = (Clay_Color){20, 20, 20, 255};
const Clay_Color ColorButtonHover = (Clay_Color){40, 40, 40, 255};
const Clay_Color ColorText = (Clay_Color){193, 68, 14, 255};
const Clay_Color ColorBg = (Clay_Color){69, 24, 4, 255};
const Clay_Color ColorBlack = (Clay_Color){0, 0, 0, 255};

// =================================
//            FONT IDS
// =================================
const uint32_t FontIdNormal16 = 0;
const uint32_t FontIdNormal24 = 1;
const uint32_t FontIdBold16 = 2;
const uint32_t FontIdBold24 = 3;
const uint32_t FontIdItalic16 = 4;
const uint32_t FontIdItalic24 = 5;
const uint32_t FontIdTitle = 6;

// =================================
//      ELEMENT CONFIGURATIONS
// =================================
const Clay_TextElementConfig HeaderTextConfig = (Clay_TextElementConfig){.fontId = FontIdTitle, .fontSize = 36, .textColor = ColorText};
const Clay_TextElementConfig FooterTextConfig = (Clay_TextElementConfig){.fontId = FontIdBold24, .fontSize = 24, .textColor = ColorText};
const Clay_TextElementConfig ButtonTextConfig = (Clay_TextElementConfig){.fontId = FontIdNormal24, .fontSize = 24, .textColor = ColorText};
const Clay_LayoutConfig buttonLayoutConfig = {
    .sizing = {
        .width = CLAY_SIZING_GROW(),
        .height = CLAY_SIZING_FIXED(50),
    },
    .childAlignment = {.y = CLAY_ALIGN_Y_CENTER, .x = CLAY_ALIGN_X_CENTER},
};

// =================================
//          ELEMENT IDs
// =================================
typedef enum IDS
{
  PowerOnButtonID = 0,
  PowerThrustersButtonID,
  CollectSamplesButtonID,
  EmergencyShutdownButtonID,
  InitDescendButtonID,
  InitManualControlButtonID,
  ArrowRightButtonID,
  ArrowLeftButtonID,
  ArrowUpButtonID,
} IDS;

// 16:9 resolution
const float ScreenWidth = 1400.0f; // UPDATED
const float ScreenHeight = 850.0f; // UPDATED

typedef void (*ButtonCallback)(void);

Texture rightArrowTexture;
Texture leftArrowTexture;
Texture upArrowTexture;
const float ArrowImageSize = 80;

char *dyn_string;

// =================================
//          BUTTON CALLBACKS
// =================================
void power_on_button(void)
{
  printf("Power ON\n");
}

void power_thrusters_button(void)
{
  printf("Power thrusters\n");
}

void collect_samples_button(void)
{
  printf("Collect samples\n");
}

void emergency_shutdown_button(void)
{
  printf("Emergency shutdown\n");
}

void init_descend_button(void)
{
  printf("Init descend\n");
}

void init_manual_control_button(void)
{
  printf("Init Manual Control\n");
}

void up_arrow_button(void)
{
  lunar_lander_set_thruster_force(xy(0, -200));
  // printf("up arrow button!\n");
}

void left_arrow_button(void)
{
  lunar_lander_set_thruster_force(xy(-50, 0));
}

void right_arrow_button(void)
{
  lunar_lander_set_thruster_force(xy(50, 0));
}

void handle_button_interaction(Clay_ElementId element_id, Clay_PointerData pointerInfo, intptr_t userData)
{
  if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME)
  {
    ButtonCallback cb = (ButtonCallback)userData;
    if (cb)
    {
      cb();
    }
  }
}

void handle_thrust_button_interaction(Clay_ElementId element_id, Clay_PointerData pointerInfo, intptr_t userData)
{
  ButtonCallback cb = (ButtonCallback)userData;
  if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED)
  {
    cb();
  }
  else if (pointerInfo.state == CLAY_POINTER_DATA_RELEASED)
  {
    lunar_lander_set_thruster_force(xy(0, 0));
  }
}

#define BUTTON_ID "button"
void button_element(int idx, Clay_String text, ButtonCallback callback)
{
  CLAY({
      .id = CLAY_IDI(BUTTON_ID, idx),
      .layout = buttonLayoutConfig,
      .backgroundColor = Clay_Hovered() ? ColorButtonHover : ColorButton,
      .cornerRadius = CLAY_CORNER_RADIUS(8),
  })
  {
    Clay_OnHover(handle_button_interaction, (intptr_t)callback);
    CLAY_TEXT(text, CLAY_TEXT_CONFIG(ButtonTextConfig));
  }
}

void arrow_button_element(int idx, Texture *texture, ButtonCallback callback)
{
  CLAY({
      .id = CLAY_IDI(BUTTON_ID, idx),
      .layout = {.sizing = {.width = CLAY_SIZING_FIXED(90), .height = CLAY_SIZING_FIXED(90)}, .padding = CLAY_PADDING_ALL(8)},
      .backgroundColor = Clay_Hovered() ? ColorButtonHover : ColorButton,
      .cornerRadius = CLAY_CORNER_RADIUS(8),
  })
  {
    Clay_OnHover(handle_thrust_button_interaction, (intptr_t)callback);
    CLAY({
        .id = CLAY_IDI("ArrowButtonImage", idx),
        .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}},
        .image = {.imageData = texture},
    })
    {
    }
  }
}

Clay_RenderCommandArray build_layout(void)
{
  // Create layout
  Clay_BeginLayout();

  // CLAY(CLAY_ID("Main"), CLAY_LAYOUT(---), CLAY_RECTANGLE({.color = ColorBg}))
  CLAY({
      .id = CLAY_ID("Main"),
      .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = CLAY_PADDING_ALL(8), .childGap = 16},
      .backgroundColor = ColorBg,
  })
  {
    // CLAY(CLAY_ID("Header"),
    //      CLAY_LAYOUT(---),
    //      CLAY_RECTANGLE({.color = ColorLight, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    CLAY({
        .id = CLAY_ID("Header"),
        .layout = {
            .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(50)},
            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
            .padding = CLAY_PADDING_ALL(8),
        },
        .backgroundColor = ColorLight,
        .cornerRadius = CLAY_CORNER_RADIUS(8),
    })
    {
      CLAY_TEXT(CLAY_STRING("Mars Rover Control Panel"), CLAY_TEXT_CONFIG(HeaderTextConfig));
    }

    // CLAY(CLAY_ID("CenterContainer"), CLAY_LAYOUT(---), CLAY_RECTANGLE({.color = ColorBg}))
    CLAY({
        .id = CLAY_ID("CenterContainer"),
        .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .childGap = 16},
        .backgroundColor = ColorBg,
    })
    {
      // CLAY(CLAY_ID("SideBar1"),
      //      CLAY_LAYOUT(),
      //      CLAY_RECTANGLE({.color = ColorLight, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      CLAY({
          .id = CLAY_ID("SideBar1"),
          .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = CLAY_PADDING_ALL(8), .childGap = 16},
          .backgroundColor = ColorLight,
          .cornerRadius = CLAY_CORNER_RADIUS(8),
      })
      {
        // When id's are button_1, button_2, button_3, button_4, they all seem to get the same hash?
        // Should investigate the exact output of the clay hashing function
        button_element(PowerOnButtonID, CLAY_STRING("Power On"), power_on_button);
        button_element(PowerThrustersButtonID, CLAY_STRING("Power Thrusters"), power_thrusters_button);
        button_element(CollectSamplesButtonID, CLAY_STRING("Collect Samples"), collect_samples_button);
        button_element(EmergencyShutdownButtonID, CLAY_STRING("Emergency Shutdown"), emergency_shutdown_button);
      }

      // CLAY(CLAY_ID("MainContent"),
      //      CLAY_LAYOUT(---),
      //      CLAY_RECTANGLE({.color = ColorBlack, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      CLAY({
          .id = CLAY_ID("GameView"),
          .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}},
          .backgroundColor = ColorBlack,
          .cornerRadius = CLAY_CORNER_RADIUS(8),
      })
      {
        mainLayoutElement = Clay__GetOpenLayoutElement();
      }

      // CLAY(CLAY_ID("SideBar2"),
      //      CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16, .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = 0}}),
      //      CLAY_RECTANGLE({.color = ColorLight, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
      CLAY({
          .id = CLAY_ID("SideBar2"),
          .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = CLAY_PADDING_ALL(8), .childGap = 16, .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = 0}},
          .backgroundColor = ColorLight,
          .cornerRadius = CLAY_CORNER_RADIUS(8),
      })
      {
        arrow_button_element(ArrowUpButtonID, &upArrowTexture, up_arrow_button);
        CLAY({
            .id = CLAY_ID("ArrowButtonRow"),
            .layout = {.layoutDirection = CLAY_LEFT_TO_RIGHT, .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(100)}, .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = 0}, .childGap = 78},
        })
        {
          arrow_button_element(ArrowLeftButtonID, &leftArrowTexture, left_arrow_button);
          arrow_button_element(ArrowRightButtonID, &rightArrowTexture, right_arrow_button);
        }
        button_element(InitDescendButtonID, CLAY_STRING("Init Descend"), init_descend_button);
        button_element(InitManualControlButtonID, CLAY_STRING("Init Manual Control"), init_manual_control_button);
      }
    }

    // CLAY(CLAY_ID("Footer"),
    //      CLAY_LAYOUT(---),
    //      CLAY_RECTANGLE({.color = ColorLight, .cornerRadius = {8.0f, 8.0f, 8.0f, 8.0f}}))
    CLAY({
        .id = CLAY_ID("Footer"),
        .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(200)}, .childAlignment = {0, 0}, .childGap = 16, .padding = {32}},
        .backgroundColor = ColorLight,
        .cornerRadius = CLAY_CORNER_RADIUS(8),
    })
    {
      CLAY_TEXT(CLAY_STRING(""), CLAY_TEXT_CONFIG(FooterTextConfig));
      CLAY_TEXT(CLAY_STRING("> Samples Collected by Rover"), CLAY_TEXT_CONFIG(FooterTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power Thrusters"), CLAY_TEXT_CONFIG(FooterTextConfig));
      CLAY_TEXT(CLAY_STRING("> issued command: Power On"), CLAY_TEXT_CONFIG(FooterTextConfig));
    }
  }

  return Clay_EndLayout();
}

void insert_font(Font *fonts, uint32_t font_id, int font_size, const char *path)
{
  fonts[font_id] = LoadFontEx(path, font_size * 2, NULL, 0);
  SetTextureFilter(fonts[font_id].texture, TEXTURE_FILTER_TRILINEAR);
}

void handle_clay_error(Clay_ErrorData errorData)
{
  printf("%s", errorData.errorText.chars);
}

int main(void)
{
  // Init raylib
  Clay_Raylib_Initialize((int)ScreenWidth, (int)ScreenHeight, "Mars Rover Control Panel", FLAG_BORDERLESS_WINDOWED_MODE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

  // Init Clay
  uint64_t clay_memory_size = Clay_MinMemorySize();
  Clay_Arena memory_arena = {
      .memory = malloc(clay_memory_size),
      .capacity = clay_memory_size,
  };
  Clay_Dimensions dimensions = {
      .width = ScreenWidth,
      .height = ScreenHeight,
  };
  Clay_Initialize(memory_arena, dimensions, (Clay_ErrorHandler){handle_clay_error});

  SetTargetFPS(144);

  Font fonts[16];
  insert_font(fonts, FontIdNormal16, 16, "resources/0xProto-Regular.ttf");
  insert_font(fonts, FontIdNormal24, 24, "resources/0xProto-Regular.ttf");
  insert_font(fonts, FontIdBold16, 16, "resources/0xProto-Bold.ttf");
  insert_font(fonts, FontIdBold24, 24, "resources/0xProto-Bold.ttf");
  insert_font(fonts, FontIdItalic16, 16, "resources/0xProto-Italic.ttf");
  insert_font(fonts, FontIdItalic24, 24, "resources/0xProto-Italic.ttf");
  insert_font(fonts, FontIdTitle, 36, "resources/0xProto-Regular.ttf");
  Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

  rightArrowTexture = LoadTextureFromImage(LoadImage("resources/arrow-right.png"));
  leftArrowTexture = LoadTextureFromImage(LoadImage("resources/arrow-left.png"));
  upArrowTexture = LoadTextureFromImage(LoadImage("resources/arrow-up.png"));

  // Main application loop
  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();

    // Update states
    Vector2 raylibMousePosition = GetMousePosition();
    Clay_Vector2 mousePosition = {
        .x = raylibMousePosition.x,
        .y = raylibMousePosition.y,
    };
    Clay_SetPointerState(mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

    Vector2 raylibMouseWheelDelta = GetMouseWheelMoveV();
    Clay_Vector2 mouseWheelDelta = {
        .x = raylibMouseWheelDelta.x,
        .y = raylibMouseWheelDelta.y,
    };
    Clay_UpdateScrollContainers(true, mouseWheelDelta, dt);

    Clay_RenderCommandArray renderCommands = build_layout();

    // Create the game texture
    if (mainLayoutElement != NULL && !game_window.has_loaded_game_texture)
    {
      Clay_LayoutElementHashMapItem *mapItem = Clay__GetHashMapItem(mainLayoutElement->id);
      Clay_BoundingBox elementBox = mapItem->boundingBox;
      game_window.window_rect.width = elementBox.width;
      game_window.window_rect.height = elementBox.height;
      game_window.window_rect.x = elementBox.x;
      game_window.window_rect.y = elementBox.y;
      game_window.game_texture = LoadRenderTexture(elementBox.width, elementBox.height);
      game_window.has_loaded_game_texture = true;

      lunar_lander_init(game_window.window_rect.width, game_window.window_rect.height);
    }

    if (game_window.has_loaded_game_texture)
    {
      lunar_lander_update(dt);

      // Render game scene to the render texture
      BeginTextureMode(game_window.game_texture);
      ClearBackground(BLACK); // Background color of the game
      lunar_lander_render();
      EndTextureMode();
    }

    // Render stuff
    BeginDrawing();

    ClearBackground(WHITE);
    Clay_Raylib_Render(renderCommands, fonts);

    if (game_window.has_loaded_game_texture)
    {
      BeginScissorMode(game_window.window_rect.x, game_window.window_rect.y, game_window.window_rect.width, game_window.window_rect.height);
      DrawTexturePro(game_window.game_texture.texture,
                     (Rectangle){0, 0, game_window.game_texture.texture.width, -game_window.game_texture.texture.height}, // Flip vertically
                     game_window.window_rect,
                     (Vector2){0, 0},
                     0.0f,
                     WHITE);
      EndScissorMode();
    }

    EndDrawing();
  }

  UnloadRenderTexture(game_window.game_texture);
  CloseWindow();

  return 0;

  return 0;
}