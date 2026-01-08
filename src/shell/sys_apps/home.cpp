#include "home.h"

Home::Home() {
    label_params = LabelParamInit((LabelParams){
        .bounds = global_state->viewport,
        .flags = LABEL_FONT_MULT | LABEL_H_ALIGN | LABEL_V_ALIGN,
        .font_mult = 10,
        .h_align = TEXT_ALIGN_CENTER,
        .v_align = TEXT_ALIGN_MIDDLE
    });
}

Home::~Home() {}

void Home::update() {}

void Home::draw() {
    label_params.text = global_state->GetCurrentTime().c_str();

    global_state->api->Label(&label_params);
}
