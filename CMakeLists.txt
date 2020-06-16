cmake_minimum_required(VERSION 3.16.5)

set( CMAKE_VERBOSE_MAKEFILE on )

# TODO(wmmc88): disable in tree builds

include(toolchain.cmake)
include(mbed-os.cmake)

project(MarsRoverFirmware)

#add_executable(${PROJECT_NAME} apps/test-blinky/main.cpp)
#target_link_libraries(${PROJECT_NAME} hello)


