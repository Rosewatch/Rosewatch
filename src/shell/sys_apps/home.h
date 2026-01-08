#pragma once

#include "raygui.h"
#include "../sys/global.h"

class Home {
private:
    LabelParams label_params;

public:
    Home();
    ~Home();

    void update();

    void draw();
};