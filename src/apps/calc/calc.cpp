#include "api/app_interface.h"
#include "shared/app_api.h"
#include <iostream>
using namespace std;

ButtonParams button_params;
LabelParams label_params;

int counter = 0;

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

    label_params = api->LabelParamInit((LabelParams){
        .text = "0",
        .bounds = (Rectangle) {
            .x = 200,
            .y = 100,
            .width = 100,
            .height = 100
        }
    });
}

extern "C" void update(AppAPI* api) {}

extern "C" void draw(AppAPI* api) {
    if (api->Button(&button_params)) {
        counter++;
        api->print("Calc button pressed!");
    }

    char c = char(counter);
    label_params.text = &c;
    api->Label(&label_params);
}
