file(GLOB HEADERS
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

add_library(camera SHARED ${HEADERS} camera.cpp)

set_property(TARGET camera PROPERTY ENABLE_EXPORTS ON)
target_include_directories(camera PRIVATE "${PROJECT_SOURCE_DIR}/include")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(camera PRIVATE ${OpenCV_LIBS})

install(TARGETS camera
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/apps"
    ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/apps"
)

add_custom_target(install_camera)
add_custom_command(
    TARGET install_camera POST_BUILD
    COMMAND bash -c "make install" #libXYZ.so
)