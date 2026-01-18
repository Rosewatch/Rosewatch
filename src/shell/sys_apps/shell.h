#pragma once

#include "../sys/global.h"

#define WIDTH 150
#define CLOCK_HEIGHT 75
#define ELEMENT_MARGIN_B 5
#define APP_OFFSET (WIDTH/2 + CLOCK_HEIGHT + (ELEMENT_MARGIN_B * 2))

class Shell {
private:
    int open_amount;

public:
    Shell();
    ~Shell();

    void update();
    void draw();
};
