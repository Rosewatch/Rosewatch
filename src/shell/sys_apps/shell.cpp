#include "shell.h"
#include "../sys/runtime_app.h"

Shell::Shell() {
    global_state->SetViewportDimensions(
        screenWidth - WIDTH,
        screenHeight
    );

    panel_width = WIDTH;
    panel_height = screenHeight;

    clock_width = WIDTH;
    clock_height = CLOCK_HEIGHT;

    clock_params = LabelParamInit((LabelParams) {
        .text = "",
        .flags = LABEL_font_size | LABEL_H_ALIGN | LABEL_V_ALIGN,
        .font_size = 32,
        .h_align = CENTER,
        .v_align = CENTER
    });

    home_params = ButtonParamInit((ButtonParams) {
        .text = "Home",
        .flags = BUTTON_FONT_SIZE,
        .font_size = 32
    });
    
    drawer_params = ButtonParamInit((ButtonParams) {
        .text = "Apps",
        .flags = BUTTON_FONT_SIZE,
        .font_size = 32
    });

    apps_params = std::vector<ShellApp>();
    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        apps_params.push_back((ShellApp) {
            .param = ButtonParamInit((ButtonParams) {
                .text = (*it).title.c_str(),
                .flags = BUTTON_FONT_SIZE,
                .font_size = 32
            }),
            .close_param = ButtonParamInit((ButtonParams) {
                .text = "X",
                .flags = BUTTON_FONT_SIZE,
                .font_size = 32
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
    CLAY(CLAY_ID("Shell Sidebar"), (Clay_ElementDeclaration) {
        .layout = {
            .sizing = { .width = CLAY_SIZING_FIXED(WIDTH), .height = CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = (Clay_Color) {224, 215, 210, 255},
    }) {
        if (global_state->api->Button(&home_params)) {
            global_state->SetApplicationState(APPLICATION_HOME);
        }

        if (global_state->api->Button(&drawer_params)) {
            global_state->SetApplicationState(APPLICATION_APPS);
        }
    }

    // Vertical line
    //DrawLine(panel_rect.x, panel_rect.y, panel_rect.x, panel_rect.height, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    
    //clock_params.text = global_state->GetCurrentTime().c_str();
    //global_state->api->Label(&clock_params);

    // Time differ line
    /*DrawLine(clock_rect.x,
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
    }*/
}
