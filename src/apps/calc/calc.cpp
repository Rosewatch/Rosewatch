#include "api/app_interface.h"
#include "shared/app_api.h"
#include "shared/gui.h"

#include <string>

static int counter = 0;

extern "C" void init(AppAPI* api) {
    api->print("Calc plugin loaded");
}

extern "C" void update(AppAPI* api) {
    (void)api;
}

extern "C" void draw(AppAPI* api) {
    api->BeginBox((GuiBoxParams){
        .padding = 8,
        .background = GUI_COLOR(30,30,30,255),
    });

    if (api->Button((GuiButtonParams){ .text = "Hello!" })) {
        counter++;
    }

    api->Label((GuiLabelParams){
        .text = "OK",
        .font_size = 24,
    });

    api->End();
}

