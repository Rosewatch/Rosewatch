#include "api/app_interface.h"
#include "shared/app_api.h"

ButtonParams button_params;

extern "C" void init(AppAPI* api) {
    api->print("Hello from plugin init()");

    button_params = api->ButtonParamInit((ButtonParams){
        .text = "Hello!",
        .rect = (Rectangle) {
            .x = 100,
            .y = 100,
            .width = 100,
            .height = 100
        }
    });
}

extern "C" void update(AppAPI* api) {}

extern "C" void draw(AppAPI* api) {
    if (api->Button(&button_params)) {
        api->print("Calc button pressed!");
    }
}
