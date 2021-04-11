add_library(mbed-uwrt-nucleo INTERFACE)

target_include_directories(mbed-uwrt-nucleo
        INTERFACE
        include
        )

target_link_libraries(mbed-uwrt-nucleo INTERFACE mbed-nucleo-f446re)
target_compile_options(mbed-uwrt-nucleo INTERFACE -w) # Disable all warnings from mbed target code
