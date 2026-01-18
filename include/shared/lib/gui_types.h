#ifndef GUI_TYPES_H
#define GUI_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
   GUI-owned public types (plugin-safe)
   ============================================================ */

/* ---------------- Colors ---------------- */

typedef struct GuiColor {
    unsigned char r, g, b, a;
} GuiColor;

/* ---------------- Theme ---------------- */

typedef struct Theme {
    GuiColor bg_col;
    GuiColor bg_hover_col;
    GuiColor fg_col;
    GuiColor border_col;

    GuiFont fonts[3];
} Theme;

/* ============================================================
   Raylib conversion helpers
   Enabled ONLY when raylib is included
   ============================================================ */

#ifdef RAYLIB_H

/* GuiColor -> raylib Color */
static inline Color GuiColor_ToRaylib(GuiColor c)
{
    return (Color){ c.r, c.g, c.b, c.a };
}

/* raylib Color -> GuiColor */
static inline GuiColor GuiColor_FromRaylib(Color c)
{
    return (GuiColor){ c.r, c.g, c.b, c.a };
}

/* Convenience macros */
#define RL_COLOR(c)   GuiColor_ToRaylib(c)
#define GUI_COLOR(c)  GuiColor_FromRaylib(c)

/* Fonts (opaque) */
Font Gui_GetRaylibFont(GuiFont font);
#define RL_FONT(f) Gui_GetRaylibFont(f)

#endif /* RAYLIB_H */

/* ============================================================
   Clay conversion helpers
   Enabled ONLY when GUI_USE_CLAY is defined
   ============================================================ */

#ifdef GUI_USE_CLAY

/* GuiColor -> Clay_Color (normalized floats) */
static inline Clay_Color GuiColor_ToClay(GuiColor c)
{
    return (Clay_Color){
        c.r / 255.0f,
        c.g / 255.0f,
        c.b / 255.0f,
        c.a / 255.0f
    };
}

/* Clay_Color -> GuiColor */
static inline GuiColor GuiColor_FromClay(Clay_Color c)
{
    return (GuiColor){
        (unsigned char)(c.r * 255.0f),
        (unsigned char)(c.g * 255.0f),
        (unsigned char)(c.b * 255.0f),
        (unsigned char)(c.a * 255.0f)
    };
}

/* Convenience macros */
#define CLAY_COLOR(c) GuiColor_ToClay(c)
#define GUI_COLOR_FROM_CLAY(c) GuiColor_FromClay(c)

#endif /* GUI_USE_CLAY */

#ifdef __cplusplus
}
#endif

#endif /* GUI_TYPES_H */
