#pragma once

#include "gui.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AppAPI {
    uint32_t struct_size;

    void (*print)(const char* text);
    bool (*Button)(const ButtonParams* params);
    ButtonParams (*ButtonParamInit)(ButtonParams params);
    void (*Label)(const LabelParams* params);
    LabelParams (*LabelParamInit)(LabelParams params);
} AppAPI;

void AppAPI_Init(AppAPI* api);

#ifdef __cplusplus
} // extern "C"
#endif