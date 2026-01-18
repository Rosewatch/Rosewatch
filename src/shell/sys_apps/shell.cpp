#include "shell.h"

#include "../sys/runtime_app.h"
#include "../sys/global.h"

#include "shared/gui.h"

/* ============================================================
   Lifecycle
   ============================================================ */

Shell::Shell() {
    global_state->SetViewportDimensions(
        screenWidth - WIDTH,
        screenHeight
    );
}

Shell::~Shell() {}

/* ============================================================
   Update
   ============================================================ */

void Shell::update() {
    open_amount = 0;

    for (auto& app : *global_state->apps) {
        if (app.open)
            open_amount++;
    }
}

/* ============================================================
   Draw
   ============================================================ */

void Shell::draw() {
    /* ------------------------------------------------------------
       Sidebar
       ------------------------------------------------------------ */

    Gui_BeginBox({
        .size_x     = GUI_SIZING_FIXED(WIDTH),
        .size_y     = GUI_SIZING_GROW(),
        .direction  = GUI_DIR_START, /* column */
        .padding    = 6,
        .background = global_state->current_theme->bg_col,
    });

    GuiButtonParams p = GUI_BUTTON_DEFAULTS;
    p.text = "Home";
    p.size_x = GUI_SIZING_GROW();
    p.size_y = GUI_SIZING_FIXED(48);
    p.h_align = GUI_DIR_CENTER;
    p.v_align = GUI_DIR_CENTER;
    p.font_size = 32;

    if (Button(p)) {
        global_state->SetApplicationState(APPLICATION_HOME);
    }

    if (Button({
            .text      = "Apps",
            .size_x    = GUI_SIZING_GROW(),
            .size_y    = GUI_SIZING_FIXED(48),
            .h_align   = GUI_DIR_CENTER,
            .v_align   = GUI_DIR_CENTER,
            .font_size = 32,
        })) {
        global_state->SetApplicationState(APPLICATION_APPS);
    }

    Gui_End();

    /* ------------------------------------------------------------
       Clock
       ------------------------------------------------------------ */

    Label({
        .text      = global_state->GetCurrentTime().c_str(),
        .size_x    = GUI_SIZING_FIXED(WIDTH),
        .size_y    = GUI_SIZING_FIXED(CLOCK_HEIGHT),
        .h_align   = GUI_DIR_CENTER,
        .v_align   = GUI_DIR_CENTER,
        .font_size = 32,
    });

    /* ------------------------------------------------------------
       Open applications
       ------------------------------------------------------------ */

    int index = 0;

    for (auto& app : *global_state->apps) {
        if (!app.open) {
            index++;
            continue;
        }

        Gui_BeginBox({
            .size_x     = GUI_SIZING_FIXED(WIDTH),
            .size_y     = GUI_SIZING_FIXED(40),
            .direction  = GUI_DIR_END, /* row */
            .padding    = 4,
            .background = global_state->current_theme->bg_col,
        });

        if (Button({
                .text      = app.title.c_str(),
                .size_x    = GUI_SIZING_GROW(),
                .size_y    = GUI_SIZING_FIXED(40),
                .h_align   = GUI_DIR_START,
                .v_align   = GUI_DIR_CENTER,
                .font_size = 20,
            })) {
            focus_app(index);
        }

        if (Button({
                .text      = "X",
                .size_x    = GUI_SIZING_FIXED(30),
                .size_y    = GUI_SIZING_FIXED(40),
                .h_align   = GUI_DIR_CENTER,
                .v_align   = GUI_DIR_CENTER,
                .font_size = 20,
            })) {
            close_app(index);
        }

        Gui_End();
        index++;
    }
}
