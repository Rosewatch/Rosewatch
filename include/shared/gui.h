#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include "lib/clay.h"
#include "raylib.h"

enum Direction {
    START,
    CENTER,
    END
};

enum Sizing {
    FIT,
    GROW,
    PERCENT,
    FIXED
};

typedef struct Dimensions {
    float width;
    float height;
} Dimension;

// For now we only support 3 fonts per theme
#define FONT_REGULAR 0
#define FONT_ITALIC 1
#define FONT_BOLD 2

typedef struct {
    Clay_Color bg_col, bg_hover_col, fg_col, border_col;
    Font fonts[3];
} Theme;

extern Theme* current_theme;
std::vector<Theme> load_themes();

typedef enum ButtonFlags {
    BUTTON_NONE        = 0,
    BUTTON_FONT_SIZE   = 1 << 0,
    BUTTON_H_ALIGN     = 1 << 1,
    BUTTON_V_ALIGN     = 1 << 2,
    BUTTON_SIZING_X    = 1 << 3,
    BUTTON_SIZING_Y    = 1 << 4
} ButtonFlags;

extern bool down_last_frame;
bool MouseClicked();
bool MouseJustClicked();
Clay_String clay_string_from_cstr(const char* cstr);

struct ButtonParams {
    const char* text;

    uint32_t struct_size;
    uint32_t flags;

    float font_size;
    Direction h_align;
    Direction v_align;
    Sizing sizing_x, sizing_y;
    float sizing_num_x, sizing_num_y;
};
bool Button(const ButtonParams* params);

ButtonParams ButtonParamInit(ButtonParams in_params);

void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);


typedef enum LabelFlags {
    LABEL_NONE        = 0,
    LABEL_font_size   = 1 << 0,
    LABEL_H_ALIGN     = 1 << 1,
    LABEL_V_ALIGN     = 1 << 2,
    LABEL_SIZING_X    = 1 << 3,
    LABEL_SIZING_Y    = 1 << 4
} LabelFlags;

struct LabelParams {
    const char* text;

    uint32_t struct_size;
    uint32_t flags;

    float font_size;
    Direction h_align;
    Direction v_align;
    Sizing sizing_x, sizing_y;
    float sizing_num_x, sizing_num_y;
};
void Label(const LabelParams* params);

LabelParams LabelParamInit(LabelParams in_params);
