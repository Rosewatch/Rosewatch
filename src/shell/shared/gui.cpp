#include "shared/app_api.h"

Rectangle GetAlignedRect(
    float w, float h,
    Direction x_dir,
    Direction y_dir,
    Rectangle parent
) {
    Rectangle rect = {};

    if (x_dir == Direction::START) {
        rect.x = parent.x;
    } else if (x_dir == Direction::CENTER) {
        rect.x = parent.x/2 - w/2;
    } else if (x_dir == Direction::END) {
        rect.x = parent.x - w;
    }

    if (y_dir == Direction::START) {
        rect.y = parent.y;
    } else if (y_dir == Direction::CENTER) {
        rect.y = parent.y/2 - h/2;
    } else if (x_dir == Direction::END) {
        rect.y = parent.y - h;
    }

    rect.width = w;
    rect.height = h;
    
    return rect;
}

bool Button(const ButtonParams* params) {
    if (params->flags & BUTTON_FONT_MULT) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize * params->font_mult);
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

    if (params->flags & BUTTON_FONT_MULT) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
    }
}

ButtonParams ButtonParamInit(ButtonParams in_params) {
    ButtonParams params = in_params;
    params.struct_size = sizeof(params);
    return params;
}

void Label(const LabelParams* params) {
    if (params->flags & LABEL_FONT_MULT) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize * params->font_mult);
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

    if (params->flags & LABEL_FONT_MULT) {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
    }
}

LabelParams LabelParamInit(LabelParams in_params) {
    LabelParams params = in_params;
    params.struct_size = sizeof(params);
    return params;
}
