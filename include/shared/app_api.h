#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
   Shared basic types
   ============================================================ */

typedef struct Dimensions {
    float width;
    float height;
} Dimensions;

/* ============================================================
   GUI API
   ============================================================ */

#include "gui.h"

/* ============================================================
   App API
   ============================================================ */

typedef struct AppAPI {
   uint32_t struct_size;

   /* ------------------------------------------------------------
   Logging
   ------------------------------------------------------------ */
   void (*print)(const char* text);
   void (*Error)(const char* id, const char* error);

   /* ------------------------------------------------------------
   GUI widgets (pass-by-value)
   ------------------------------------------------------------ */
   int  (*Button)(GuiButtonParams params);
   void (*Label)(GuiLabelParams params);

   /* Layout (raylib-style, Clay-safe) */
   void (*BeginBox)(GuiBoxParams params);
   void (*End)(void);

   /* ------------------------------------------------------------
   GUI images (opaque, backend-owned)
   ------------------------------------------------------------ */
   GuiImage* (*ImageCreate)(int width, int height);
   void (*ImageUpdate)(GuiImage* image, const void* rgba_pixels);
   void (*ImageDraw)(GuiImage* image, int x, int y);
   void (*ImageDrawTinted)(GuiImage* image, int x, int y, GuiColor tint);
   void (*ImageDestroy)(GuiImage* image);

   /* ------------------------------------------------------------
   System
   ------------------------------------------------------------ */
   const char* (*GetCurrentTimeCString)(void);
   Dimensions (*GetViewport)(void);

   /* ------------------------------------------------------------
   Memory
   ------------------------------------------------------------ */
   void* (*MemAlloc)(uint32_t size);
   void  (*MemFree)(void* ptr);

} AppAPI;

/* ============================================================
   Init
   ============================================================ */

void AppAPI_Init(AppAPI* api);

#ifdef __cplusplus
}
#endif
