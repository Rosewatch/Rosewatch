#include "home.h"

Home::Home() {
    label_params = LabelParamInit((LabelParams){
        .flags = LABEL_font_size,
        .font_size = 128
    });
}

Home::~Home() {}

void Home::update() {}

void Home::draw() {
    label_params.text = global_state->GetCurrentTime().c_str();

    global_state->api->Label(&label_params);
}
