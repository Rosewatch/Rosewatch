#include <iostream>
#include "shared/gui.h"
#include "shared/app_api.h"
#include "../sys/global.h"

// Concrete API implementation
void api_print(const char* text) {
    std::cout << "[API] " << text << std::endl;
}

void api_error(const char* id, const char* error) {
    std::cout << "[ERROR] " << error << std::endl;
    // TODO: Close app? just don't kill OS
}

std::string api_getcurrenttime() {
    return global_state->GetCurrentTime();
}

Rectangle api_getviewport() {
    return global_state->viewport;
}

extern "C" void AppAPI_Init(AppAPI* api) {
    if (!api) return;

    api->struct_size = sizeof(AppAPI);
    api->print = api_print;
    api->Error = api_error;

    api->Button = Button;
    api->ButtonParamInit = ButtonParamInit;

    api->GetCurrentTime = api_getcurrenttime;
    api->GetViewport = api_getviewport;

    api->Label = Label;
    api->LabelParamInit = LabelParamInit;

    api->LoadTextureFromImage = LoadTextureFromImage;
    api->UpdateTexture = UpdateTexture;
    api->DrawTexture = DrawTexture;

    api->MemAlloc = MemAlloc;
    api->MemFree = MemFree;
}
