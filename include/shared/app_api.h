#pragma once

#include "gui.h"
#include "lib/raylib.h"
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AppAPI {
    uint32_t struct_size;

    void (*print)(const char* text);
    void (*Error)(const char* id, const char* error);

    bool (*Button)(const ButtonParams* params);
    ButtonParams (*ButtonParamInit)(ButtonParams params);
    
    void (*Label)(const LabelParams* params);
    LabelParams (*LabelParamInit)(LabelParams params);
    
    Texture2D (*LoadTextureFromImage)(Image image);
    void (*UpdateTexture)(Texture2D texture, const void* pixels);
    void (*DrawTexture)(Texture2D texture, int posX, int posY, Color tint);

    std::string (*GetCurrentTime)();
    Dimensions (*GetViewport)();

    void* (*MemAlloc)(unsigned int size);
    void (*MemFree)(void* ptr);
} AppAPI;

void AppAPI_Init(AppAPI* api);

#ifdef __cplusplus
} // extern "C"
#endif
