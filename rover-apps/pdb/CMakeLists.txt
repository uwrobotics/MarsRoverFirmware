add_library(PDB_Monitoring STATIC)
target_sources(PDB_Monitoring PRIVATE src/PDB_Module.cpp)
target_include_directories(PDB_Monitoring PUBLIC include)
target_link_libraries(PDB_Monitoring
        PRIVATE
        mbed-os
        )

add_executable(pdb)
target_sources(pdb PRIVATE ../common/src/main.cpp)
target_include_directories(pdb PUBLIC ../common/include)
target_link_libraries(pdb
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        /* Modules */
        PDB_Monitoring
        )
mbed_set_post_build(pdb)
