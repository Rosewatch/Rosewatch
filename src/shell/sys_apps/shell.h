#pragma once

#include "raygui.h"
#include "../sys/global.h"

#define WIDTH 150
#define CLOCK_HEIGHT 75
#define CLOCK_MARGIN_B 5

class Shell {
private:
    Rectangle panel_rect, clock_rect;
    LabelParams clock_params;
    ButtonParams home_params, drawer_params, app_one_rparams, app_two_params, app_three_params;

public:
    Shell();
    ~Shell();

    void update();

    void draw();
};