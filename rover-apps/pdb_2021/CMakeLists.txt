add_library(PDB_Monitoring STATIC)
target_sources(PDB_Monitoring PRIVATE src/PDBMonitoring.cpp)
target_include_directories(PDB_Monitoring PUBLIC
        include
        ../common/include
        )
target_link_libraries(PDB_Monitoring
        PRIVATE
        Logger
        mbed-os
        )

add_executable(pdb_2021)
target_sources(pdb_2021 PRIVATE ../common/src/main.cpp)
target_include_directories(pdb_2021 PUBLIC include ../common/include)
target_link_libraries(pdb_2021
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        #Modules
        PDB_Monitoring
        )
mbed_set_post_build(pdb_2021)
