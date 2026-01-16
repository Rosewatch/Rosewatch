#include "home.h"

Home::Home() {
    label_params = LabelParamInit((LabelParams){
        .width = global_state->viewport.width,
        .height = global_state->viewport.height,
        .flags = LABEL_font_size | LABEL_H_ALIGN | LABEL_V_ALIGN,
        .font_size = 10,
        .h_align = CENTER,
        .v_align = CENTER
    });
}

Home::~Home() {}

void Home::update() {}

void Home::draw() {
    label_params.text = global_state->GetCurrentTime().c_str();

    global_state->api->Label(&label_params);
}
