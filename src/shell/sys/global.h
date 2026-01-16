#pragma once

#include "shared/app_api.h"
#include "runtime_app.h"
#include <vector>
#include <memory>

#include <ctime>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

// Replace with a settings thingy later...
#define TIME_FORMAT "%H:%M"

typedef enum {
    APPLICATION_NONE,
    APPLICATION_HOME,
    APPLICATION_APPS,
    APPLICATION_APP,
} ApplicationState;

class GlobalState {
public:
    Dimensions viewport;
    ApplicationState app_state;
    std::vector<RuntimeApplication> *apps;
    AppAPI* api{};
    time_t raw_time;

    GlobalState(AppAPI* api);

    void SetViewportDimensions(float width, float height);

    void SetApplicationState(ApplicationState new_state);
    
    void SetApps(std::vector<RuntimeApplication>* new_apps);

    void tick();

    string DateTime(time_t time, const char* format);
    string GetCurrentTime();
};

extern const int screenWidth;
extern const int screenHeight;

extern GlobalState* global_state;