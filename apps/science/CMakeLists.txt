add_executable(science.${TARGET}-board.elf)
target_sources(science.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(science.${TARGET}-board.elf 
    PRIVATE
    CANMsg
    ActuatorController
    ScienceConfig
    Encoder
    LimServo
    MoistureSensor
    EncoderRelative_Quadrature
    CANBus
    uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(science.${TARGET}-board.elf)

add_library(ScienceConfig INTERFACE)
target_include_directories(ScienceConfig INTERFACE include)
