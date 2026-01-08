#pragma once

#include <vector>
#include "../sys/global.h"
#include "raygui.h"

#define TILE_SIZE 150

#define GRID_WIDTH 7
#define GRID_PAD_X 10
#define GRID_PAD_T 25

class Drawer {
public:
    Drawer();
    ~Drawer();

    void update();

    void draw();
};