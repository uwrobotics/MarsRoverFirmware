add_executable(test-netzer-encoder.${TARGET}-board.elf)
target_sources(test-netzer-encoder.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-netzer-encoder.${TARGET}-board.elf 
  PRIVATE 
  NetzerEncoder
)
target_set_firmware_properties(test-netzer-encoder.${TARGET}-board.elf)

add_library(NetzerEncoder STATIC)
target_sources(NetzerEncoder PRIVATE src/NetzerEncoder.cpp)
target_include_directories(NetzerEncoder PUBLIC include)
target_set_mbed_dependency(NetzerEncoder)
