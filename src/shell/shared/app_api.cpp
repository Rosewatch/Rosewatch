#include <iostream>
#include "shared/gui.h"
#include "shared/app_api.h"

// Concrete API implementation
void api_print(const char* text) {
    std::cout << "[API] " << text << std::endl;
}

extern "C" void AppAPI_Init(AppAPI* api) {
    if (!api) return;

    api->struct_size = sizeof(AppAPI);
    api->print = api_print;
    api->Button = Button;
    api->ButtonParamInit = ButtonParamInit;
    api->Label = Label;
    api->LabelParamInit = LabelParamInit;
}
