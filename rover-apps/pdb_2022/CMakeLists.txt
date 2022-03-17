add_library(PDB_Monitoring2022 STATIC)
target_sources(PDB_Monitoring2022 PRIVATE src/PDBMonitoring.cpp)
target_include_directories(PDB_Monitoring2022 PUBLIC
        include
        ${ROVER_APPS_DIR}/common/include
        )
target_link_libraries(PDB_Monitoring2022
        PRIVATE
        Logger
        mbed-os
        )

add_executable(pdb_2022)
target_sources(pdb_2022 PRIVATE ${ROVER_APPS_DIR}/common/src/main.cpp)
target_include_directories(pdb_2022 PUBLIC include ${ROVER_APPS_DIR}/common/include)
target_link_libraries(pdb_2022
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        #Modules
        PDB_Monitoring2022
        )
mbed_set_post_build(pdb_2022)
