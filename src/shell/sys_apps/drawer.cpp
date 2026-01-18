#include "drawer.h"
#include "../sys/global.h"
#include "shared/gui.h"

Drawer::Drawer() {}

Drawer::~Drawer() {}

void Drawer::update() {}

void Drawer::draw() {
    int i = 0;

    Gui_BeginBox({
        .size_x = GUI_SIZING_GROW(),
        .size_y = GUI_SIZING_GROW(),
        .padding = 8,
        .background = global_state->current_theme->bg_col,
    });

    for (auto& app : *global_state->apps) {
        if (Button({
                .text = app.title.c_str(),
                .font_size = 30,
            })) {
            open_app(i);
        }

        i++;
    }

    Gui_End();
}
