add_executable(test-netzer-encoder.${TARGET}-board.elf)
target_sources(test-netzer-encoder.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-netzer-encoder.${TARGET}-board.elf 
  PRIVATE 
  Netzer
)
target_set_firmware_properties(test-netzer-encoder.${TARGET}-board.elf)

add_library(Netzer STATIC)
target_sources(Netzer PRIVATE src/Netzer.cpp)
target_include_directories(Netzer PUBLIC include)
target_set_mbed_dependency(Netzer)
