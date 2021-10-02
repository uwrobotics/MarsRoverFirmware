add_executable(pdb_2021)
target_sources(pdb_2021 PRIVATE src/main.cpp)
target_include_directories(pdb_2021 PUBLIC include)
target_link_libraries(pdb_2021
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(pdb_2021)
