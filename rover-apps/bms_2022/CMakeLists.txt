add_library(BMS_Monitoring STATIC)
target_sources(BMS_Monitoring PRIVATE src/BMSMonitoring.cpp)
target_include_directories(BMS_Monitoring PUBLIC
        include
        ${ROVER_APPS_DIR}/common/include
        )
target_link_libraries(BMS_Monitoring
        PRIVATE
        Logger
        mbed-os
        )

add_executable(bms_2022)
target_sources(bms_2022 PRIVATE ${ROVER_APPS_DIR}/common/src/main.cpp)
target_include_directories(bms_2022 PUBLIC include ${ROVER_APPS_DIR}/common/include)
target_link_libraries(bms_2022
        PRIVATE
        CANBus
        CANMsg
        Logger
        uwrt-mars-rover-hw-bridge
        #Modules
        BMS_Monitoring
        )
mbed_set_post_build(bms_2022)
