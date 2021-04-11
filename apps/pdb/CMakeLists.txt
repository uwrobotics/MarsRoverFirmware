add_executable(pdb)
target_sources(pdb PRIVATE src/main.cpp)
target_include_directories(pdb PUBLIC include)
target_link_libraries(pdb
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(pdb)
