file(GLOB HEADERS
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

add_library(calc SHARED ${HEADERS} calc.cpp)

set_property(TARGET calc PROPERTY ENABLE_EXPORTS ON)
target_include_directories(calc PRIVATE "${PROJECT_SOURCE_DIR}/include")

install(TARGETS calc
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/apps"
    ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/apps"
)

add_custom_target(install_calc)
add_custom_command(
    TARGET install_calc POST_BUILD
    COMMAND bash -c "make install" #libXYZ.so
)