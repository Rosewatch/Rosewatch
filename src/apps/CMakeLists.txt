add_subdirectory(calc)
target_include_directories(calc PRIVATE ${CMAKE_SOURCE_DIR}/raygui)
add_dependencies(install_apps install_calc)
