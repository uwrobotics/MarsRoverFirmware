add_executable(tutorial-servo-pot-control.${TARGET}-board.elf)
target_sources(tutorial-servo-pot-control.${TARGET}-board.elf PRIVATE
src/main.cpp)
target_link_libraries(tutorial-servo-pot-control.${TARGET}-board.elf
	CANBus
	CANMsg
	uwrt-mars-rover-hw-bridge
	TutorialServo
	)
target_set_firmware_properties(tutorial-servo-pot-control.${TARGET}-board.elf)