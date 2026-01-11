#include "api/app_interface.h"
#include "shared/app_api.h"
#include "shared/gui.h"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

const char* id = "Camera";

typedef struct {
    cv::VideoCapture gCam;
    Texture2D texture = { 0 };
    int width  = 0;
    int height = 0;
} CaptureCamera;

CaptureCamera InitCamera(int index, AppAPI* api) {
    CaptureCamera camera = CaptureCamera {};

    camera.gCam = cv::VideoCapture(index);

    camera.width  = (int)camera.gCam.get(cv::CAP_PROP_FRAME_WIDTH);
    camera.height = (int)camera.gCam.get(cv::CAP_PROP_FRAME_HEIGHT);

    // Create empty texture (RGB)
    Image img = {
        .data = api->MemAlloc(camera.width * camera.height * 3),
        .width = camera.width,
        .height = camera.height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8
    };

    camera.texture = api->LoadTextureFromImage(img);
    api->MemFree(img.data);

    return camera;
}

int CountCameras() {
    cv::VideoCapture temp_camera;
    int maxTested = 10;
    for (int i = 0; i < maxTested; i++) {
        cv::VideoCapture temp_camera(i);
        bool res = (!temp_camera.isOpened());
        temp_camera.release();
        if (res) {
            return i;
        }
    }
    return maxTested;
}

cv::VideoWriter writer;
bool recording = false;
int camera_amount;
int cam_index = 0;
std::vector<CaptureCamera> cameras;
std::vector<ButtonParams> camera_params;
ButtonParams record_button;
LabelParams error_label;

extern "C" void init(AppAPI* api) {
    Rectangle viewport = api->GetViewport();
    error_label = api->LabelParamInit((LabelParams) {
        .text = "No webcams detected",
        .bounds = viewport,
    });

    camera_amount = CountCameras();
    if (!camera_amount) {
        api->Error(id, "Failed to open webcam");
        return;
    }

    cameras = std::vector<CaptureCamera>();
    for (int i = 0; i < camera_amount; i++) {
        CaptureCamera camera = InitCamera(i, api);
        if (camera.gCam.isOpened()) {
            cameras.push_back(camera);
        }
    }

    
    record_button = api->ButtonParamInit((ButtonParams){
        .text = "Record",
        .rect = (Rectangle) {
            .x = viewport.width - 110,
            .y = 10,
            .width = 100,
            .height = 50,
        }
    });

    api->print(std::to_string(camera_amount).c_str());

    camera_params = std::vector<ButtonParams>();
    for (int i = 0; i < camera_amount; i++) {
        camera_params.push_back(api->ButtonParamInit((ButtonParams) {
            .text = std::to_string(i).c_str(),
            .rect = (Rectangle) {
                .x = (float)(60 * i) + 10,
                .y = viewport.height - 60,
                .width = 50,
                .height = 50,
            }
        }));
    }
}

extern "C" void update(AppAPI* api) {
    if (camera_amount) {
        int i = 0;
        for(auto camera : cameras) {
            cv::Mat frame;
            camera.gCam >> frame;
            if (frame.empty()) return;

            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

            if (recording && cam_index == i) {
                writer.write(frame);
            }

            api->UpdateTexture(camera.texture, frame.data);
            
            i++;
        }
    }
}

extern "C" void draw(AppAPI* api) {
    if (!camera_amount) {
        api->Label(&error_label);
    } else {
        int i = 0;
        for(auto camera : cameras) {
            if (cam_index == i) {
                api->DrawTexture(camera.texture, 10, 10, WHITE);
            }
            
            i++;
        }

        for(auto camera_param : camera_params) {
            if (api->Button(&camera_param)) {
                cam_index = i;
            }
        }

        if (api->Button(&record_button)) {
            recording = !recording;

            if (recording) {
                std::string preface = "output_";
                std::string time = api->GetCurrentTime();
                std::string postface = ".avi";
                CaptureCamera camera = (*(cameras.begin() + cam_index));

                writer.open(
                    (preface + time + postface).c_str(),
                    cv::VideoWriter::fourcc('M','J','P','G'),
                    15,
                    cv::Size(camera.width, camera.height)
                );
            } else {
                writer.release();
            }
        }
    }
}
