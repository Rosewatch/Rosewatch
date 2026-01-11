add_subdirectory(calc)
target_include_directories(calc PRIVATE ${CMAKE_SOURCE_DIR}/raygui)
add_dependencies(install_apps install_calc)

add_subdirectory(camera)
target_include_directories(camera PRIVATE ${CMAKE_SOURCE_DIR}/raygui)
add_dependencies(install_apps install_camera)