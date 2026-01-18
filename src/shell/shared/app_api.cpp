#include <iostream>
#include <string>
#include <cstdlib>

#include "shared/app_api.h"
#include "shared/gui.h"
#include "../sys/global.h"

/* ============================================================
   Logging
   ============================================================ */

static void api_print(const char* text) {
    std::cout << "[API] " << text << std::endl;
}

static void api_error(const char* id, const char* error) {
    std::cerr << "[ERROR][" << id << "] " << error << std::endl;
}

/* ============================================================
   Time adapter (C++ → C)
   ============================================================ */

static const char* api_get_current_time_cstr(void) {
    static std::string cached;
    cached = global_state->GetCurrentTime();
    return cached.c_str();
}

/* ============================================================
   Viewport
   ============================================================ */

static Dimensions api_get_viewport(void) {
    return global_state->viewport;
}

/* ============================================================
   Memory adapters (ABI-safe)
   ============================================================ */

static void* api_mem_alloc(uint32_t size) {
    return malloc((size_t)size);
}

static void api_mem_free(void* ptr) {
    free(ptr);
}


/* ============================================================
   AppAPI Init
   ============================================================ */

extern "C" void AppAPI_Init(AppAPI* api) {
    if (!api) return;

    api->struct_size = sizeof(AppAPI);

    /* ------------------------------------------------------------
       Logging
       ------------------------------------------------------------ */
    api->print = api_print;
    api->Error = api_error;

    /* ------------------------------------------------------------
       GUI widgets
       ------------------------------------------------------------ */
    api->Button   = Button;
    api->Label    = Label;
    
    api->BeginBox = Gui_BeginBox;
    api->End         = Gui_End;


    /* ------------------------------------------------------------
       GUI images
       ------------------------------------------------------------ */
    api->ImageCreate     = GuiImage_Create;
    api->ImageUpdate     = GuiImage_Update;
    api->ImageDraw       = GuiImage_Draw;
    api->ImageDrawTinted = GuiImage_DrawTinted;
    api->ImageDestroy    = GuiImage_Destroy;

    /* ------------------------------------------------------------
       System
       ------------------------------------------------------------ */
    api->GetCurrentTimeCString = api_get_current_time_cstr;
    api->GetViewport           = api_get_viewport;

    /* ------------------------------------------------------------
       Memory
       ------------------------------------------------------------ */
    api->MemAlloc = api_mem_alloc;
    api->MemFree  = api_mem_free;

}
