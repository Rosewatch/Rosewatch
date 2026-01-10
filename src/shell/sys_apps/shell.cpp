#include "shell.h"
#include "../sys/runtime_app.h"

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
            .y = CLOCK_HEIGHT + ELEMENT_MARGIN_B,
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
            .y = CLOCK_HEIGHT + ELEMENT_MARGIN_B,
            .width = WIDTH/2,
            .height = WIDTH/2,
        },
        .flags = BUTTON_FONT_MULT | BUTTON_H_ALIGN | BUTTON_V_ALIGN,
        .font_mult = 2,
        .h_align = TEXT_ALIGN_CENTER,
        .v_align = TEXT_ALIGN_MIDDLE
    });

    apps_params = std::vector<ShellApp>();
    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        apps_params.push_back((ShellApp) {
            .param = ButtonParamInit((ButtonParams) {
                .text = (*it).title.c_str(),
                .rect = (Rectangle) {
                    .x = screenWidth - WIDTH,
                    .y = CLOCK_HEIGHT + ELEMENT_MARGIN_B,
                    .width = WIDTH - WIDTH/4,
                    .height = WIDTH/2,
                },
                .flags = BUTTON_FONT_MULT | BUTTON_H_ALIGN | BUTTON_V_ALIGN,
                .font_mult = 2,
                .h_align = TEXT_ALIGN_LEFT,
                .v_align = TEXT_ALIGN_MIDDLE,
            }),
            .close_param = ButtonParamInit((ButtonParams) {
                .text = "X",
                .rect = (Rectangle) {
                    .x = screenWidth - WIDTH/4,
                    .y = CLOCK_HEIGHT + ELEMENT_MARGIN_B,
                    .width = WIDTH/4,
                    .height = WIDTH/2,
                },
                .flags = BUTTON_FONT_MULT | BUTTON_H_ALIGN | BUTTON_V_ALIGN,
                .font_mult = 2,
                .h_align = TEXT_ALIGN_CENTER,
                .v_align = TEXT_ALIGN_MIDDLE,
            }),
            .open = &it->open,
        });
    }

    open_amount = 0;
}

Shell::~Shell() {}

void Shell::update() {
    open_amount = 0;
    for (auto it = apps_params.begin(); it != apps_params.end(); ++it) {
        if (*(*it).open) {
            open_amount++;
        }
    }
}

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

    int open_index = 0;
    int index = 0;
    for (auto it = apps_params.begin(); it != apps_params.end(); ++it) {
        if (*(*it).open && open_index != open_amount) {
            int y = (WIDTH/2 * open_index) + APP_OFFSET;
            it->param.rect.y = y;
            it->close_param.rect.y = y;

            if (global_state->api->Button(&(*it).param)) {
                focus_app(index);
            }

            if (global_state->api->Button(&(*it).close_param)) {
                close_app(index);
            }

            open_index++;
        }

        index++;
    }
}
