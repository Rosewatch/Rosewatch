#include "shared/gui.h"

/* ============================================================
   Backend-only includes
   ============================================================ */

#include "raylib.h"
#include "shared/lib/clay.h"

#include <vector>
#include <cstring>
#include <fstream>
#include <filesystem>

#include "shared/lib/json.hpp"
#include "../sys/global.h"

using json = nlohmann::json;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

/* ============================================================
   Backend font wrapper (opaque to frontend)
   ============================================================ */

struct GuiFontHandle {
    Font font;
};

/* ============================================================
   Image handle
   ============================================================ */

struct GuiImage {
    Texture2D texture;
    int width;
    int height;
};

/* ============================================================
   Globals
   ============================================================ */

Theme* current_theme = nullptr;
static bool mouse_down_last = false;

/* ============================================================
   Input helpers
   ============================================================ */

static int MouseDown(void) {
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

static int MousePressed(void) {
    int down = MouseDown();
    int pressed = down && !mouse_down_last;
    mouse_down_last = down;
    return pressed;
}

/* ============================================================
   Clay helpers
   ============================================================ */

static Clay_String clay_string(const char* s) {
    return Clay_String{
        .length = (int32_t)strlen(s),
        .chars  = s
    };
}

static Clay_Color to_clay(GuiColor c) {
    return Clay_Color{
        c.r / 255.0f,
        c.g / 255.0f,
        c.b / 255.0f,
        c.a / 255.0f
    };
}

static Color to_raylib(GuiColor c) {
    return Color{ c.r, c.g, c.b, c.a };
}

static Clay_SizingAxis to_clay(GuiSizing s) {
    switch (s.kind) {
        case GUI_SIZING_GROW:    return CLAY_SIZING_GROW(0);
        case GUI_SIZING_PERCENT: return CLAY_SIZING_PERCENT(s.value);
        case GUI_SIZING_FIXED:
        case GUI_SIZING_FIT:
        default:                return CLAY_SIZING_FIXED(s.value);
    }
}

static Clay_LayoutAlignmentX to_align_x(GuiDirection d) {
    return d == GUI_DIR_START  ? CLAY_ALIGN_X_LEFT  :
           d == GUI_DIR_END    ? CLAY_ALIGN_X_RIGHT :
                                 CLAY_ALIGN_X_CENTER;
}

static Clay_LayoutAlignmentY to_align_y(GuiDirection d) {
    return d == GUI_DIR_START  ? CLAY_ALIGN_Y_TOP    :
           d == GUI_DIR_END    ? CLAY_ALIGN_Y_BOTTOM :
                                 CLAY_ALIGN_Y_CENTER;
}


/* ============================================================
   Theme loading
   ============================================================ */

static GuiColor load_color(const json& j) {
    return GuiColor{
        (uint8_t)j[0],
        (uint8_t)j[1],
        (uint8_t)j[2],
        (uint8_t)j[3]
    };
}

Theme* load_themes(uint32_t* out_count) {
    static std::vector<Theme> themes;
    static std::vector<GuiFontHandle> fonts;

    themes.clear();
    fonts.clear();

    for (auto& entry : recursive_directory_iterator("assets/themes")) {
        std::ifstream f(entry.path());
        if (!f) continue;

        json data = json::parse(f, nullptr, false);
        if (data.is_discarded()) continue;

        Theme t{};
        t.bg_col       = load_color(data["bg_col"]);
        t.bg_hover_col = load_color(data["bg_hover_col"]);
        t.fg_col       = load_color(data["fg_col"]);
        t.border_col   = load_color(data["border_col"]);

        auto load_font = [&](const char* name) {
            fonts.push_back({});
            fonts.back().font =
                LoadFontEx(TextFormat("assets/fonts/%s", name), 64, 0, 400);
            return &fonts.back();
        };

        t.fonts[FONT_REGULAR] =
            load_font(data.value("font_regular", "AdwaitaMono-Regular.ttf").c_str());
        t.fonts[FONT_ITALIC] =
            load_font(data.value("font_italic", "AdwaitaMono-Regular.ttf").c_str());
        t.fonts[FONT_BOLD] =
            load_font(data.value("font_bold", "AdwaitaMono-Regular.ttf").c_str());

        themes.push_back(t);
    }

    if (out_count) *out_count = (uint32_t)themes.size();
    return themes.data();
}

/* ============================================================
   Widgets
   ============================================================ */

int Button(GuiButtonParams params) {
    GuiButtonParams p = GUI_BUTTON_DEFAULTS;
    p = params;

    int clicked = 0;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = { to_clay(p.size_x), to_clay(p.size_y) },
            .padding = CLAY_PADDING_ALL(6),
            .childAlignment = {
                to_align_x(p.h_align),
                to_align_y(p.v_align)
            }
        },
        .backgroundColor = to_clay(
            Clay_Hovered()
                ? current_theme->bg_hover_col
                : current_theme->bg_col
        ),
        .border = {
            .color = to_clay(current_theme->border_col)
        }
    }) {
        if (Clay_Hovered() && MousePressed())
            clicked = 1;

        CLAY_TEXT(
            clay_string(p.text),
            CLAY_TEXT_CONFIG({
                .textColor = to_clay(current_theme->fg_col),
                .fontId    = FONT_REGULAR,
                .fontSize  = (uint16_t)p.font_size,
            })
        );
    }

    return clicked;
}

void Label(GuiLabelParams params) {
    GuiLabelParams p = GUI_LABEL_DEFAULTS;
    p = params;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = { to_clay(p.size_x), to_clay(p.size_y) },
            .childAlignment = {
                to_align_x(p.h_align),
                to_align_y(p.v_align)
            }
        }
    }) {
        CLAY_TEXT(
            clay_string(p.text),
            CLAY_TEXT_CONFIG({
                .textColor = to_clay(current_theme->fg_col),
                .fontId    = FONT_REGULAR,
                .fontSize  = (uint16_t)p.font_size,
            })
        );
    }
}

void Gui_BeginBox(GuiBoxParams params) {
    GuiBoxParams p = GUI_BOX_DEFAULTS;
    p = params;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = { to_clay(p.size_x), to_clay(p.size_y) },
            .padding = CLAY_PADDING_ALL(p.padding),
        },
        .backgroundColor = to_clay(p.background),
    }) {
        /* scope open */
    }
}

void Gui_End(void) {
    /* Clay auto-closes scopes */
}

/* ============================================================
   Images
   ============================================================ */

GuiImage* GuiImage_Create(int width, int height) {
    Image img = GenImageColor(width, height, BLANK);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    GuiImage* gimg = (GuiImage*)MemAlloc(sizeof(GuiImage));
    gimg->texture = tex;
    gimg->width   = width;
    gimg->height  = height;
    return gimg;
}

void GuiImage_Update(GuiImage* image, const void* rgba_pixels) {
    if (!image) return;
    UpdateTexture(image->texture, rgba_pixels);
}

void GuiImage_Draw(GuiImage* image, int x, int y) {
    if (!image) return;
    DrawTexture(image->texture, x, y, WHITE);
}

void GuiImage_DrawTinted(GuiImage* image, int x, int y, GuiColor tint) {
    if (!image) return;
    DrawTexture(image->texture, x, y, to_raylib(tint));
}

void GuiImage_Destroy(GuiImage* image) {
    if (!image) return;
    UnloadTexture(image->texture);
    MemFree(image);
}

/* ============================================================
   Clay ↔ Raylib font bridge (FIXED)
   ============================================================ */

Font* Gui_GetRaylibFonts(const Theme* theme) {
    static Font raylib_fonts[3];

    for (int i = 0; i < 3; ++i) {
        raylib_fonts[i] = theme->fonts[i]->font;
    }

    return raylib_fonts;
}
