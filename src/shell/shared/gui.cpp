#include "shared/app_api.h"
#include <iostream>
#include <cstring>

typedef struct ButtonData {
    bool clicked;
} ButtonData;

bool down_last_frame = false;

bool MouseClicked() {
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

bool MouseJustClicked() {
    if (MouseClicked()) {
        if (down_last_frame) {
            return false;
        } else {
            down_last_frame = true;
            return true;
        }
    } else {
        down_last_frame = false;
        return false;
    }
}

Clay_String string_from_cstr(const char* cstr) {
    return (Clay_String) {
        .length = strlen(cstr),
        .chars = cstr
    };
}

bool Button(const ButtonParams* params) {
    bool clicked = false;

    CLAY(CLAY_ID("Button"), {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) },
            .padding = CLAY_PADDING_ALL(8),
        },
        .backgroundColor = Clay_Hovered()
            ? (Clay_Color){224, 215, 0, 255}
            : (Clay_Color){224, 0, 215, 255},
    }) {

        if (Clay_Hovered() && MouseJustClicked()) {
            clicked = true;
        }

        CLAY_TEXT(string_from_cstr(params->text), CLAY_TEXT_CONFIG({
            .textColor = {255, 255, 255, 255},
            .fontId = 0,
            .fontSize = params->flags & BUTTON_FONT_SIZE
                ? (uint16_t)params->font_size
                : 16,
        }));
    }

    return clicked;
}



    /*if (params->flags & BUTTON_FONT_SIZE) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize * params->font_size);
    }

    if (params->flags & BUTTON_H_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, params->h_align);
    }

    if (params->flags & BUTTON_V_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, params->v_align);
    }

    return GuiButton(params->rect, params->text);

    if (params->flags & BUTTON_V_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);
    }

    if (params->flags & BUTTON_H_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    }

    if (params->flags & BUTTON_FONT_SIZE) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
    }*/

ButtonParams ButtonParamInit(ButtonParams in_params) {
    ButtonParams params = in_params;
    params.struct_size = sizeof(params);
    return params;
}

void Label(const LabelParams* params) {
    /*if (params->flags & LABEL_font_size) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize * params->font_size);
    }

    if (params->flags & LABEL_H_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, params->h_align);
    }

    if (params->flags & LABEL_V_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, params->v_align);
    }

    GuiLabel(params->bounds, params->text);

    if (params->flags & LABEL_V_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);
    }

    if (params->flags & LABEL_H_ALIGN) {
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    }

    if (params->flags & LABEL_font_size) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
    }*/
}

LabelParams LabelParamInit(LabelParams in_params) {
    LabelParams params = in_params;
    params.struct_size = sizeof(params);
    return params;
}
