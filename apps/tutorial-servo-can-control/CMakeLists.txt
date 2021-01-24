add_executable(tutorial-servo-can-control.${TARGET}-board.elf)
target_sources(tutorial-servo-can-control.${TARGET}-board.elf PRIVATE
src/main.cpp)
target_link_libraries(tutorial-servo-can-control.${TARGET}-board.elf
	TutorialServo
	CANMsg
	uwrt-mars-rover-hw-bridge
	CANBus
	)
target_set_firmware_properties(tutorial-servo-can-control.${TARGET}-board.elf)