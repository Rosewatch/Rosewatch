#pragma once

#include "lib/raygui.h"
#include <memory>
#include <cstdint>

enum Direction {
    START,
    CENTER,
    END
};


Rectangle GetAlignedRect(
    float w, float h,
    Direction x_dir,
    Direction y_dir,
    Rectangle parent
);


typedef enum ButtonFlags {
    BUTTON_NONE        = 0,
    BUTTON_FONT_MULT   = 1 << 0,
    BUTTON_H_ALIGN     = 1 << 1,
    BUTTON_V_ALIGN     = 1 << 2
} ButtonFlags;

struct ButtonParams {
    const char* text;
    Rectangle rect;

    uint32_t struct_size;
    uint32_t flags;

    float font_mult;
    GuiTextAlignment h_align;
    GuiTextAlignmentVertical v_align;
};
bool Button(const ButtonParams* params);

ButtonParams ButtonParamInit(ButtonParams in_params);


typedef enum LabelFlags {
    LABEL_NONE        = 0,
    LABEL_FONT_MULT   = 1 << 0,
    LABEL_H_ALIGN     = 1 << 1,
    LABEL_V_ALIGN     = 1 << 2
} LabelFlags;

struct LabelParams {
    const char* text;
    Rectangle bounds;

    uint32_t struct_size;
    uint32_t flags;

    float font_mult;
    GuiTextAlignment h_align;
    GuiTextAlignmentVertical v_align;
};
void Label(const LabelParams* params);

LabelParams LabelParamInit(LabelParams in_params);
