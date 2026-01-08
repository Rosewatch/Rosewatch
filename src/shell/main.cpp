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

#include "shared/app_api.h"

#include "sys_apps/drawer.h"
#include "sys_apps/home.h"
#include "sys_apps/shell.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define SHADER_ENABLED 1

int main(int argc, char **argv) {
    // Init
    InitWindow(screenWidth, screenHeight, "rosewatch");
    SetTargetFPS(15);

    AppAPI api{};
    AppAPI_Init(&api);
    GlobalState state = GlobalState(&api);
    global_state = &state;

    std::vector<RuntimeApplication> apps = std::vector<RuntimeApplication>();
    
    for (const auto& dirEntry : recursive_directory_iterator("apps")) {
        setup_app(dirEntry.path().c_str(), &apps);
    }

    global_state->SetApps(&apps);

    GuiLoadStyle("assets/terminal.rgs");
    Shader shader = LoadShader(TextFormat("assets/crt_%i.vs", GLSL_VERSION), TextFormat("assets/crt_%i.fs", GLSL_VERSION));
    int timeLoc = GetShaderLocation(shader, "time");

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

        // Enable drawing to texture
        BeginTextureMode(target);
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        ApplicationState state = global_state->app_state;

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
        EndTextureMode();

        float t = GetTime();
        SetShaderValue(shader, timeLoc, &t, SHADER_UNIFORM_FLOAT);

        // Draw
        BeginDrawing();
        if (SHADER_ENABLED) {
            BeginShaderMode(shader);
        }
        ClearBackground(RAYWHITE);

        // Render generated texture using selected postprocessing shader
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTextureRec(
            target.texture,
            (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height},
            (Vector2){0, 0},
            WHITE
        );

        if (SHADER_ENABLED) {
            EndShaderMode();
        }
        EndDrawing();
    }

    for(RuntimeApplication& app : apps) {
        dlclose(app.handle);
    }

    UnloadShader(shader);
    // Unload render texture
    UnloadRenderTexture(target);

    CloseWindow();
    

    return 0;
}