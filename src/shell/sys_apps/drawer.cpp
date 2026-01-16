#include "drawer.h"

#include "../sys/global.h"

Drawer::Drawer() {}

Drawer::~Drawer() {}

void Drawer::update() {}

void Drawer::draw() {
    int i = 0;

    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        ButtonParams params = ButtonParamInit((ButtonParams){
            .text = (*it).title.c_str(),
            .width = TILE_SIZE,
            .height = TILE_SIZE,
            .flags = BUTTON_FONT_SIZE,
            .font_size = 1.5
        });

        if (global_state->api->Button(&params)) {
            open_app(i);
        }

        i++;
    }
}
