cmake_minimum_required(VERSION 3.19)

project(traintechdata LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(traintechdata

main.cpp
mainwindow.cpp
mainwindow.h
mainwindow.ui
)

target_link_libraries(traintechdata
PRIVATE
Qt6::Core
Qt6::Widgets
)

install(TARGETS traintechdata
BUNDLE DESTINATION .
RUNTIME DESTINATION bin
)

qt_generate_deploy_app_script(
TARGET traintechdata
OUTPUT_SCRIPT deploy_script
NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

