#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
   Core Types
   ============================================================ */

typedef struct GuiColor {
    uint8_t r, g, b, a;
} GuiColor;

#define GUI_COLOR(r,g,b,a) ((GuiColor){ (r), (g), (b), (a) })

/* ============================================================
   Direction / Alignment
   ============================================================ */

typedef enum {
    GUI_DIR_START,
    GUI_DIR_CENTER,
    GUI_DIR_END
} GuiDirection;

/* ============================================================
   Sizing
   ============================================================ */

typedef enum {
    GUI_SIZING_FIT,
    GUI_SIZING_GROW,
    GUI_SIZING_FIXED,
    GUI_SIZING_PERCENT
} GuiSizingKind;

typedef struct {
    GuiSizingKind kind;
    float value;
} GuiSizing;

#define GUI_SIZING_FIT()        ((GuiSizing){ GUI_SIZING_FIT, 0.0f })
#define GUI_SIZING_GROW()       ((GuiSizing){ GUI_SIZING_GROW, 0.0f })
#define GUI_SIZING_FIXED(px)    ((GuiSizing){ GUI_SIZING_FIXED, (float)(px) })
#define GUI_SIZING_PERCENT(p)   ((GuiSizing){ GUI_SIZING_PERCENT, (float)(p) })

/* ============================================================
   Theme (opaque fonts)
   ============================================================ */

typedef struct GuiFontHandle GuiFontHandle;

#define FONT_REGULAR 0
#define FONT_ITALIC  1
#define FONT_BOLD    2

typedef struct Theme {
    GuiColor bg_col;
    GuiColor bg_hover_col;
    GuiColor fg_col;
    GuiColor border_col;

    GuiFontHandle* fonts[3];
} Theme;

extern Theme* current_theme;
Theme* load_themes(uint32_t* out_count);

/* ============================================================
   Input helpers
   ============================================================ */

int MouseClicked(void);
int MouseJustClicked(void);

/* ============================================================
   Widgets (PASS BY VALUE)
   ============================================================ */

/* ---------- Button ---------- */

typedef struct {
    const char* text;
    GuiSizing size_x;
    GuiSizing size_y;
    GuiDirection h_align;
    GuiDirection v_align;
    float font_size;
} GuiButtonParams;


#define GUI_BUTTON_DEFAULTS \
((GuiButtonParams){ \
    .text = "", \
    .size_x = GUI_SIZING_GROW(), \
    .size_y = GUI_SIZING_FIXED(48), \
    .h_align = GUI_DIR_START, \
    .v_align = GUI_DIR_START, \
    .font_size = 16.0f, \
})

int Button(GuiButtonParams params);

/* ---------- Label ---------- */

typedef struct {
    const char* text;
    GuiSizing size_x;
    GuiSizing size_y;
    GuiDirection h_align;
    GuiDirection v_align;
    float font_size;
} GuiLabelParams;


#define GUI_LABEL_DEFAULTS \
((GuiLabelParams){ \
    .text = "", \
    .size_x = GUI_SIZING_FIT(), \
    .size_y = GUI_SIZING_FIT(), \
    .h_align = GUI_DIR_START, \
    .v_align = GUI_DIR_START, \
    .font_size = 16.0f, \
})

void Label(GuiLabelParams params);

/* ---------- Box ---------- */

typedef struct {
    GuiSizing     size_x;
    GuiSizing     size_y;
    GuiDirection  direction;
    uint16_t      padding;
    GuiColor      background;
} GuiBoxParams;

#define GUI_BOX_DEFAULTS \
((GuiBoxParams){ \
    .size_x = GUI_SIZING_GROW(), \
    .size_y = GUI_SIZING_GROW(), \
    .direction = GUI_DIR_START, \
    .padding = 0, \
    .background = {0,0,0,0}, \
})

void Gui_BeginBox(GuiBoxParams params);
void Gui_End(void);

/* ============================================================
   Images (opaque)
   ============================================================ */

typedef struct GuiImage GuiImage;

GuiImage* GuiImage_Create(int width, int height);
void      GuiImage_Update(GuiImage* image, const void* rgba_pixels);
void      GuiImage_Draw(GuiImage* image, int x, int y);
void      GuiImage_DrawTinted(GuiImage* image, int x, int y, GuiColor tint);
void      GuiImage_Destroy(GuiImage* image);

#ifdef __cplusplus
}
#endif

/* ============================================================
   Backend-only (Raylib bridge)
   ============================================================ */

#ifdef GUI_BACKEND
#include "raylib.h"
Font* Gui_GetRaylibFonts(const Theme* theme);
#endif
