#include "shell.h"

Shell::Shell() {
    global_state->SetViewportDimensions(
        screenWidth - WIDTH,
        screenHeight
    );

    panel_rect = (Rectangle) {
        .x = screenWidth - WIDTH,
        .y = 0,
        .width = WIDTH,
        .height = screenHeight,
    };

    clock_rect = (Rectangle) {
        .x = screenWidth - WIDTH,
        .y = 0,
        .width = WIDTH,
        .height = CLOCK_HEIGHT,
    };

    clock_params = LabelParamInit((LabelParams) {
        .text = "",
        .bounds = clock_rect,
        .flags = LABEL_FONT_MULT | LABEL_H_ALIGN | LABEL_V_ALIGN,
        .font_mult = 4,
        .h_align = TEXT_ALIGN_CENTER,
        .v_align = TEXT_ALIGN_MIDDLE
    });

    home_params = ButtonParamInit((ButtonParams) {
        .text = "Home",
        .rect = (Rectangle) {
            .x = screenWidth - WIDTH,
            .y = CLOCK_HEIGHT + CLOCK_MARGIN_B,
            .width = WIDTH/2,
            .height = WIDTH/2,
        },
        .flags = BUTTON_FONT_MULT | BUTTON_H_ALIGN | BUTTON_V_ALIGN,
        .font_mult = 2,
        .h_align = TEXT_ALIGN_CENTER,
        .v_align = TEXT_ALIGN_MIDDLE
    });
    
    drawer_params = ButtonParamInit((ButtonParams) {
        .text = "Apps",
        .rect = (Rectangle) {
            .x = screenWidth - WIDTH/2,
            .y = CLOCK_HEIGHT + CLOCK_MARGIN_B,
            .width = WIDTH/2,
            .height = WIDTH/2,
        },
        .flags = BUTTON_FONT_MULT | BUTTON_H_ALIGN | BUTTON_V_ALIGN,
        .font_mult = 2,
        .h_align = TEXT_ALIGN_CENTER,
        .v_align = TEXT_ALIGN_MIDDLE
    });
}

Shell::~Shell() {}

void Shell::update() {}

void Shell::draw() {
    // Vertical line
    DrawLine(panel_rect.x, panel_rect.y, panel_rect.x, panel_rect.height, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    
    clock_params.text = global_state->GetCurrentTime().c_str();
    global_state->api->Label(&clock_params);

    // Time differ line
    DrawLine(clock_rect.x,
        clock_rect.y + clock_rect.height,
        clock_rect.x + clock_rect.width,
        clock_rect.y + clock_rect.height,
        GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
    );

    if (global_state->api->Button(&home_params)) {
        global_state->SetApplicationState(APPLICATION_HOME);
    }

    if (global_state->api->Button(&drawer_params)) {
        global_state->SetApplicationState(APPLICATION_APPS);
    }
}
