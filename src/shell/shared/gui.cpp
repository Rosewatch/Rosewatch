#include "shared/app_api.h"
#include "../sys/global.h"
#include <iostream>
#include <cstring>

#include "shared/lib/json.hpp"
#include <fstream>

#include <filesystem>
using json = nlohmann::json;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

typedef struct ButtonData {
    bool clicked;
} ButtonData;

#define CLAY_BLACK (Clay_Color) {0,0,0,0}
Clay_Color load_color(json data) {
    return (Clay_Color) {
        data[0].get<float>(),
        data[1].get<float>(),
        data[2].get<float>(),
        data[3].get<float>(),
    };
}

Theme* current_theme = nullptr;
std::vector<Theme> load_themes() {
    std::vector<Theme> themes;

    for (const auto& dirEntry : recursive_directory_iterator("assets/themes")) {
        bool error = false;

        std::ifstream f(dirEntry.path());
        json data = json::parse(f);

        Font fonts[3];
        fonts[0] = LoadFontEx(TextFormat("assets/fonts/", (data.contains("font_regular") && data["font_regular"].is_string() ? data["font_regular"].get<std::string>().c_str() : "AdwaitaMono-Regular.ttf")), 64, 0, 400);
        //SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
        fonts[1] = LoadFontEx(TextFormat("assets/fonts/", (data.contains("font_italic") && data["font_italic"].is_string() ? data["font_italic"].get<std::string>().c_str() : "AdwaitaMono-Regular.ttf")), 64, 0, 400);
        //SetTextureFilter(fonts[1].texture, TEXTURE_FILTER_BILINEAR);
        fonts[2] = LoadFontEx(TextFormat("assets/fonts/", (data.contains("font_bold") && data["font_bold"].is_string() ? data["font_bold"].get<std::string>().c_str() : "AdwaitaMono-Regular.ttf")), 64, 0, 400);
        //SetTextureFilter(fonts[2].texture, TEXTURE_FILTER_BILINEAR);

        Theme theme = {
            .bg_col = data.contains("bg_col") && data["bg_col"].is_array() ? load_color(data["bg_col"]) : CLAY_BLACK,
            .bg_hover_col = data.contains("bg_hover_col") && data["bg_hover_col"].is_array() ? load_color(data["bg_hover_col"]) : CLAY_BLACK,
            .fg_col = data.contains("fg_col") && data["fg_col"].is_array() ? load_color(data["fg_col"]) : CLAY_BLACK,
            .border_col = data.contains("border_col") && data["border_col"].is_array() ? load_color(data["border_col"]) : CLAY_BLACK,
            .fonts = *fonts
        };
    
        if (error) {
            continue;
        } else {
            themes.push_back(theme);
        }
    }

    return themes;
}

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

Clay_String clay_string_from_cstr(const char* cstr) {
    return (Clay_String) {
        .length = strlen(cstr),
        .chars = cstr
    };
}

Clay_LayoutAlignmentX directon_to_clay_x(Direction dir) {
    if (dir == Direction::START) {
        return CLAY_ALIGN_X_LEFT;
    } else if (dir == Direction::CENTER) {
        return CLAY_ALIGN_X_CENTER;
    } else if (dir == Direction::END) {
        return CLAY_ALIGN_X_RIGHT;
    } else {
        return CLAY_ALIGN_X_CENTER;
    }
}

Clay_LayoutAlignmentY directon_to_clay_y(Direction dir) {
    if (dir == Direction::START) {
        return CLAY_ALIGN_Y_TOP;
    } else if (dir == Direction::CENTER) {
        return CLAY_ALIGN_Y_CENTER;
    } else if (dir == Direction::END) {
        return CLAY_ALIGN_Y_BOTTOM;
    } else {
        return CLAY_ALIGN_Y_CENTER;
    }
}

Clay_SizingAxis sizing_to_clay_sizing(Sizing sizing, float sizing_num) {
    if (sizing == Sizing::FIT) {
        return CLAY_SIZING_FIXED(sizing_num);
    } else if (sizing = Sizing::FIXED) {
        return CLAY_SIZING_FIXED(sizing_num);
    } else if (sizing == Sizing::GROW) {
        return CLAY_SIZING_GROW(sizing_num);
    } else if (sizing == Sizing::PERCENT) {
        return CLAY_SIZING_PERCENT(sizing_num);
    } else {
        return CLAY_SIZING_FIXED(50);
    }
}

bool Button(const ButtonParams* params) {
    bool clicked = false;
    Theme* theme = global_state->current_theme;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = {
                params->flags & LABEL_SIZING_X
                    ? sizing_to_clay_sizing(params->sizing_x, params->sizing_num_x)
                    : CLAY_SIZING_GROW(0),
                params->flags & LABEL_SIZING_Y
                    ? sizing_to_clay_sizing(params->sizing_y, params->sizing_num_y)
                    : CLAY_SIZING_FIXED(50),
            },
            .padding = CLAY_PADDING_ALL(4),
            .childAlignment = {
                .x = params->flags & BUTTON_H_ALIGN
                    ? directon_to_clay_x(params->h_align)
                    : CLAY_ALIGN_X_CENTER,
                .y = params->flags & BUTTON_V_ALIGN
                    ? directon_to_clay_y(params->h_align)
                    : CLAY_ALIGN_Y_CENTER,
            }
        },
        .backgroundColor = Clay_Hovered()
            ? theme->bg_col
            : theme->bg_hover_col,
        .border = {
            .color = theme->border_col,
            .width = { 1, 1, 1, 1, 0 },
        },
    }) {

        if (Clay_Hovered() && MouseJustClicked()) {
            clicked = true;
        }

        CLAY_TEXT(clay_string_from_cstr(params->text), CLAY_TEXT_CONFIG({
            .textColor = theme->fg_col,
            .fontId = FONT_REGULAR,
            .fontSize = params->flags & BUTTON_FONT_SIZE
                ? (uint16_t)params->font_size
                : 16,
        }));
    }

    return clicked;
}

/*if (params->flags & BUTTON_H_ALIGN) {
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, params->h_align);
}

if (params->flags & BUTTON_V_ALIGN) {
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, params->v_align);
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
