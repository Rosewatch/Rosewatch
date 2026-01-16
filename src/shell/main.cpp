#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <vector>
#include <filesystem>
#include <iostream>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define CLAY_IMPLEMENTATION
#include "shared/lib/clay.h"
#include "shared/clay_renderer_raylib.c"
#undef CLAY_IMPLEMENTATION

#include "shared/app_api.h"

#include "sys_apps/drawer.h"
#include "sys_apps/home.h"
#include "sys_apps/shell.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

#define ON_PI

// Dosen't work on pi...
#ifndef ON_PI
#define SHADER_ENABLED
#endif

#ifdef SHADER_ENABLED
    #if defined(PLATFORM_DESKTOP)
        #define GLSL_VERSION            330
    #else   // PLATFORM_ANDROID, PLATFORM_WEB
        #define GLSL_VERSION            100
    #endif
#endif

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) (Clay_Vector2) { .x = vector.x, .y = vector.y }

void HandleClayErrors(Clay_ErrorData errorData) {
    global_state->api->Error("main", errorData.errorText.chars);
}

int main(int argc, char **argv) {
    // Init
    Clay_Raylib_Initialize(screenWidth, screenHeight, "Rosewatch", FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(15);
    
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, (char *)malloc(totalMemorySize));

    Clay_Context *clayContext = Clay_Initialize(clayMemory, (Clay_Dimensions) {
       .width = (float)screenWidth,
       .height = (float)screenHeight
    }, (Clay_ErrorHandler) { HandleClayErrors });

    AppAPI api{};
    AppAPI_Init(&api);
    GlobalState state = GlobalState(&api);
    global_state = &state;

    std::vector<RuntimeApplication> apps = std::vector<RuntimeApplication>();
    
    for (const auto& dirEntry : recursive_directory_iterator("apps")) {
        setup_app(dirEntry.path().c_str(), &apps);
    }

    global_state->SetApps(&apps);

    // Clay fonts!
    Font fonts[1];
    fonts[0] = LoadFontEx("assets/AdwaitaMono-Regular.ttf", 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);


    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    GuiLoadStyle("assets/terminal.rgs");

    #ifdef SHADER_ENABLED
        Shader shader = LoadShader("assets/crt.vs", "assets/crt.fs");
        int timeLoc = GetShaderLocation(shader, "time");
    #endif

    // Create a RenderTexture2D to be used for render to texture
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    Shell shell = Shell();
    Home home = Home();
    Drawer drawer = Drawer();

    global_state->SetApplicationState(APPLICATION_HOME);

    while (!WindowShouldClose()) {
        global_state->tick();

        // Update, allows running in background
        home.update();
        drawer.update();
        shell.update();

        for(RuntimeApplication& app : apps) {
            if (app.open) {
                app.update(global_state->api);
            }
        }

        Vector2 mouseWheelDelta = GetMouseWheelMoveV();
        float mouseWheelX = mouseWheelDelta.x;
        float mouseWheelY = mouseWheelDelta.y;
        Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
        Clay_SetPointerState(mousePosition, IsMouseButtonDown(0));
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });

        // Enable drawing to texture
        BeginTextureMode(target);
        
        Clay_BeginLayout();
        Clay_SetCurrentContext(clayContext);
        //Clay_SetDebugModeEnabled(true);

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        ApplicationState state = global_state->app_state;

        CLAY_AUTO_ID({
            .layout = {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                .childAlignment = { .x = CLAY_ALIGN_X_RIGHT },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
            if (state == APPLICATION_HOME) {
                home.draw();
            } else if (state == APPLICATION_APPS) {
                drawer.draw();
            } else if (state == APPLICATION_APP) {
                for(RuntimeApplication& app : apps) {
                    if (app.active) {
                        app.draw(global_state->api);
                    }
                }
            }

            shell.draw();
        }

        Clay_Raylib_Render(Clay_EndLayout(), fonts);
        EndTextureMode();

        // Draw
        BeginDrawing();
        #ifdef SHADER_ENABLED
            BeginShaderMode(shader);
        #endif
        ClearBackground(RAYWHITE);

        // Render generated texture using selected postprocessing shader
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTextureRec(
            target.texture,
            (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height},
            (Vector2){0, 0},
            WHITE
        );

        #ifdef SHADER_ENABLED
            EndShaderMode();
        #endif
        EndDrawing();
    }

    for(RuntimeApplication& app : apps) {
        dlclose(app.handle);
    }

    #ifdef SHADER_ENABLED
    UnloadShader(shader);
    #endif
    // Unload render texture
    UnloadRenderTexture(target);
    Clay_Raylib_Close();

    return 0;
}