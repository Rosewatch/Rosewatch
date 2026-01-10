#include <string>
#include <iostream>

#include "global.h"

RuntimeApplication* setup_app(const char* path, std::vector<RuntimeApplication>* apps) {
    std::cout << "SETTING UP APPLICATION " << path << "\n";

    RuntimeApplication app = RuntimeApplication();

    app.handle = dlopen(path, RTLD_LAZY);
    if (!app.handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }

    char *error;

    app.init = (AppFPTR) dlsym(app.handle, "init");
    app.update = (AppFPTR) dlsym(app.handle, "update");
    app.draw = (AppFPTR) dlsym(app.handle, "draw");
    
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(1);
    }

    std::string title_string(path);

    auto title_index = title_string.find(".so");
    if (title_index != std::string::npos) {
        title_string.erase(title_index, title_index + 2);
    }

    auto preface_index = title_string.find("apps/");
    if (preface_index != std::string::npos) {
        title_string.erase(preface_index, preface_index + 5);
    }
    
    app.title = title_string;

    std::cout << app.title.c_str() << "\n";

    apps->push_back(app);
    
    return &apps->front();
}

RuntimeApplication* open_app(int app_index) {
    RuntimeApplication* ret_app = nullptr;

    int i = 0;
    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        if (i == app_index) {
            if (!(*it).open) {
                (*it).init(global_state->api);
            }

            (*it).open = true;
            (*it).active = true;

            ret_app = std::addressof(*it);
        } else {
            (*it).active = false;
        }
        
        i++;
    }

    global_state->SetApplicationState(APPLICATION_APP);

    return ret_app;
}
