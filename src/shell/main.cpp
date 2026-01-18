#define GUI_BACKEND

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <vector>
#include <filesystem>
#include <iostream>

/* ============================================================
   Backend includes
   ============================================================ */

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "shared/lib/clay.h"
#include "shared/clay_renderer_raylib.c"
#undef CLAY_IMPLEMENTATION

/* ============================================================
   App / GUI
   ============================================================ */

#include "shared/app_api.h"
#include "shared/gui.h"

#include "sys_apps/drawer.h"
#include "sys_apps/home.h"
#include "sys_apps/shell.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(v) ((Clay_Vector2){ (v).x, (v).y })

/* ============================================================
   Clay error handling
   ============================================================ */

void HandleClayErrors(Clay_ErrorData errorData) {
    global_state->api->Error("main", errorData.errorText.chars);
}

/* ============================================================
   Entry
   ============================================================ */

int main(int argc, char **argv) {
    /* ---------------- Window / Clay init ---------------- */

    Clay_Raylib_Initialize(screenWidth, screenHeight, "Rosewatch", FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(15);

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory =
        Clay_CreateArenaWithCapacityAndMemory(
            totalMemorySize,
            (char*)malloc(totalMemorySize)
        );

    Clay_Context* clayContext =
        Clay_Initialize(
            clayMemory,
            (Clay_Dimensions){
                .width  = (float)screenWidth,
                .height = (float)screenHeight
            },
            (Clay_ErrorHandler){ HandleClayErrors }
        );

    /* ---------------- Global state ---------------- */

    AppAPI api{};
    AppAPI_Init(&api);

    GlobalState state(&api);
    global_state = &state;

    /* ---------------- Load applications ---------------- */

    std::vector<RuntimeApplication> apps;

    for (const auto& dirEntry : recursive_directory_iterator("apps")) {
        setup_app(dirEntry.path().c_str(), &apps);
    }

    global_state->SetApps(&apps);

    /* ---------------- Load themes (NEW API) ---------------- */

    uint32_t theme_count = 0;
    Theme* themes = load_themes(&theme_count);

    global_state->SetThemes(themes, theme_count);
    global_state->SetCurrentTheme(0);

    /* ---------------- Clay font integration ----------------
       Fonts are opaque in gui.h, but backend knows them
    --------------------------------------------------------- */

    Clay_Raylib_Render(
        Clay_EndLayout(),
        Gui_GetRaylibFonts(global_state->current_theme)
    );

    /* ---------------- Render targets ---------------- */

    RenderTexture2D target =
        LoadRenderTexture(screenWidth, screenHeight);

    /* ---------------- System apps ---------------- */

    Shell  shell;
    Home   home;
    Drawer drawer;

    global_state->SetApplicationState(APPLICATION_HOME);

    /* ============================================================
       Main loop
       ============================================================ */

    while (!WindowShouldClose()) {
        global_state->tick();

        /* -------- Update -------- */

        home.update();
        drawer.update();
        shell.update();

        for (RuntimeApplication& app : apps) {
            if (app.open) {
                app.update(global_state->api);
            }
        }

        /* -------- Input -------- */

        Vector2 mouseWheelDelta = GetMouseWheelMoveV();
        (void)mouseWheelDelta; // unused for now

        Clay_SetPointerState(
            RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition()),
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)
        );

        Clay_SetLayoutDimensions(
            (Clay_Dimensions){
                (float)GetScreenWidth(),
                (float)GetScreenHeight()
            }
        );

        /* -------- Draw -------- */

        BeginDrawing();

        Clay_SetCurrentContext(clayContext);
        Clay_BeginLayout();
        // Clay_SetDebugModeEnabled(true);

        ApplicationState app_state = global_state->app_state;

        CLAY_AUTO_ID({
            .layout = {
                .sizing = {
                    CLAY_SIZING_GROW(0),
                    CLAY_SIZING_GROW(0)
                },
                .childAlignment = {
                    .x = CLAY_ALIGN_X_RIGHT
                },
            },
            .backgroundColor =
                (Clay_Color){
                    global_state->current_theme->bg_col.r / 255.0f,
                    global_state->current_theme->bg_col.g / 255.0f,
                    global_state->current_theme->bg_col.b / 255.0f,
                    global_state->current_theme->bg_col.a / 255.0f,
                }
        }) {
            if (app_state == APPLICATION_HOME) {
                home.draw();
            }
            else if (app_state == APPLICATION_APPS) {
                drawer.draw();
            }
            else if (app_state == APPLICATION_APP) {
                for (RuntimeApplication& app : apps) {
                    if (app.active) {
                        app.draw(global_state->api);
                    }
                }
            }

            shell.draw();
        }

        Clay_Raylib_Render(
            Clay_EndLayout(),
            Gui_GetRaylibFonts(global_state->current_theme)
        );

        EndDrawing();
    }

    /* ============================================================
       Shutdown
       ============================================================ */

    for (RuntimeApplication& app : apps) {
        dlclose(app.handle);
    }

    UnloadRenderTexture(target);
    Clay_Raylib_Close();

    return 0;
}
