#ifndef RAYLIB_CLAY_TYPES_H
#define RAYLIB_CLAY_TYPES_H

/* Raylib provides the canonical math + color types */
#include "raylib.h"

/* Clay consumes its own types */
#include "clay.h"

/* ============================================================
   Color
   ============================================================ */

static inline Clay_Color ClayColor_FromRaylib(Color c)
{
    return (Clay_Color){
        (float)c.r,
        (float)c.g,
        (float)c.b,
        (float)c.a
    };
}

#define CLAY_COLOR(c) ClayColor_FromRaylib(c)

/* ============================================================
   Vector2
   ============================================================ */

static inline Clay_Vector2 ClayVector2_FromRaylib(Vector2 v)
{
    return (Clay_Vector2){ v.x, v.y };
}

#define CLAY_VECTOR2(v) ClayVector2_FromRaylib(v)

/* ============================================================
   Optional helpers (common raylib sources)
   ============================================================ */

/* Mouse position */
#define CLAY_MOUSE_POSITION() \
    ClayVector2_FromRaylib(GetMousePosition())

/* Screen size */
static inline Clay_Vector2 ClayVector2_FromScreenSize(void)
{
    return (Clay_Vector2){
        (float)GetScreenWidth(),
        (float)GetScreenHeight()
    };
}

#define CLAY_SCREEN_SIZE() ClayVector2_FromScreenSize()

#endif /* RAYLIB_CLAY_TYPES_H */
