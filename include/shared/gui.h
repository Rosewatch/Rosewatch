#pragma once

#include <memory>
#include <cstdint>

#ifndef GUI_H
#include "lib/clay.h"
#define GUI_H
#endif

enum Direction {
    START,
    CENTER,
    END
};

typedef struct Dimensions {
    float width;
    float height;
} Dimension;

typedef enum ButtonFlags {
    BUTTON_NONE        = 0,
    BUTTON_FONT_SIZE   = 1 << 0,
    BUTTON_H_ALIGN     = 1 << 1,
    BUTTON_V_ALIGN     = 1 << 2
} ButtonFlags;

extern bool down_last_frame;

bool MouseClicked();
bool MouseJustClicked();
Clay_String string_from_cstr(const char* cstr);

struct ButtonParams {
    const char* text;
    float width, height;

    uint32_t struct_size;
    uint32_t flags;

    float font_size;
    Direction h_align;
    Direction v_align;
};
bool Button(const ButtonParams* params);

ButtonParams ButtonParamInit(ButtonParams in_params);

void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);


typedef enum LabelFlags {
    LABEL_NONE        = 0,
    LABEL_font_size   = 1 << 0,
    LABEL_H_ALIGN     = 1 << 1,
    LABEL_V_ALIGN     = 1 << 2
} LabelFlags;

struct LabelParams {
    const char* text;
    float width, height;

    uint32_t struct_size;
    uint32_t flags;

    float font_size;
    Direction h_align;
    Direction v_align;
};
void Label(const LabelParams* params);

LabelParams LabelParamInit(LabelParams in_params);
