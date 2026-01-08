#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <vector>
#include <string>

typedef void (*AppFPTR)(AppAPI*);

typedef struct {
    void *handle;
    bool open, active;
    std::string title;

    AppFPTR init, update, draw;
} RuntimeApplication;

RuntimeApplication* setup_app(const char* path, std::vector<RuntimeApplication>* apps);

RuntimeApplication* open_app(int app_index);
