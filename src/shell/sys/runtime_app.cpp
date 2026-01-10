#include <string>
#include <iostream>

#include "global.h"

RuntimeApplication* setup_app(const char* path, std::vector<RuntimeApplication>* apps, int index) {
    std::cout << "SETTING UP APPLICATION " << path << "\n";

    RuntimeApplication app = RuntimeApplication();
    app.path = std::string(path);

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

    if (index == -1) {
        apps->push_back(app);
        
        return &apps->front();
    } else {
        apps->at(index) = app;
        
        return &apps->at(index);
    }
}

RuntimeApplication* setup_app(const char* path, std::vector<RuntimeApplication>* apps) {
    return setup_app(path, apps, -1);
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

RuntimeApplication* focus_app(int app_index) {
    RuntimeApplication* ret_app = nullptr;

    int i = 0;
    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        if (i == app_index) {
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

RuntimeApplication* close_app(int app_index) {
    RuntimeApplication* ret_app = nullptr;

    int i = 0;
    for (auto it = global_state->apps->begin(); it != global_state->apps->end(); ++it) {
        // No matter what we'll be closing the app
        (*it).active = false;

        if (i == app_index) {
            const char* path = (*it).path.c_str();

            dlclose((*it).handle);

            setup_app(path, global_state->apps, i);

            ret_app = std::addressof(*it);
        }
        
        i++;
    }

    global_state->SetApplicationState(APPLICATION_APPS);

    return ret_app;
}

