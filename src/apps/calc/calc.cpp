#include "api/app_interface.h"
#include "shared/app_api.h"
#include <string>

const char* id = "Calc";

ButtonParams button_params;
LabelParams label_params;

int counter = 0;

extern "C" void init(AppAPI* api) {
    api->print("Hello from plugin init()");

    button_params = api->ButtonParamInit((ButtonParams){
        .text = "Hello!"
    });

    label_params = api->LabelParamInit((LabelParams){
        .text = "0"
    });
}

extern "C" void update(AppAPI* api) {}

extern "C" void draw(AppAPI* api) {
    if (api->Button(&button_params)) {
        counter++;
        api->print("Calc button pressed!");
    }

    std::string s = std::to_string(counter);
    label_params.text = s.c_str();
    api->Label(&label_params);
}
