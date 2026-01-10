#pragma once

#include "raygui.h"
#include "../sys/global.h"

#define WIDTH 150
#define CLOCK_HEIGHT 75
#define ELEMENT_MARGIN_B 5
#define APP_OFFSET (WIDTH/2 + CLOCK_HEIGHT + (ELEMENT_MARGIN_B * 2))

typedef struct ShellApp {
    ButtonParams param, close_param;
    bool* open;
} ShellApp;

class Shell {
private:
    Rectangle panel_rect, clock_rect;
    LabelParams clock_params;
    ButtonParams home_params, drawer_params;
    int open_amount;
    std::vector<ShellApp> apps_params;

public:
    Shell();
    ~Shell();

    void update();

    void draw();
};