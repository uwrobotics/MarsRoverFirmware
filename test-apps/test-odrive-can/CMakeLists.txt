add_executable(test-odrive-can)
target_sources(test-odrive-can PRIVATE src/main.cpp)
target_include_directories(test-odrive-can PUBLIC include)
target_link_libraries(test-odrive-can
	PRIVATE
	mbed-os
	CANInterface
	CANBus
	CANMsg
	uwrt-mars-rover-hw-bridge
	Logger
	)
mbed_set_post_build(test-odrive-can)