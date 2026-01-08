#pragma once

// Forward declaration of API struct
struct AppAPI;

// Application entry point
extern "C" {
void init(AppAPI* api);
void update(AppAPI* api);
void draw(AppAPI* api);
}
