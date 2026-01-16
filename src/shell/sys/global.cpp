#include "global.h"

const int screenWidth = 1280;
const int screenHeight = 720;

GlobalState* global_state = nullptr;

GlobalState::GlobalState (AppAPI* api) {
    viewport = (Dimensions) { };
    app_state = APPLICATION_HOME;
    this->api = api;
    raw_time = time(0);
}

void GlobalState::tick() {
    time(&raw_time);
}

// Function to format a time_t value into a date or time string.
string GlobalState::DateTime(time_t time, const char* format) {
    char buffer[90];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), format, timeinfo);
    return buffer;
}

string GlobalState::GetCurrentTime() {
    return DateTime(raw_time, TIME_FORMAT);
}

void GlobalState::SetViewportDimensions(float width, float height) {
    viewport.width = width;
    viewport.height = height;
}

void GlobalState::SetApplicationState(ApplicationState new_state) {
    app_state = new_state;
}

void GlobalState::SetApps(std::vector<RuntimeApplication>* new_apps) {
    apps = new_apps;
}
