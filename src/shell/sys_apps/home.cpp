#include "home.h"

Home::Home() {}

Home::~Home() {}

void Home::update() {}

void Home::draw() {
    global_state->api->Label({
        .text = global_state->GetCurrentTime().c_str(),
        .font_size = 128
    });
}
