add_executable(test-lookup-table)
target_sources(test-lookup-table PRIVATE src/main.cpp)
target_link_libraries(test-lookup-table
        PRIVATE
        LookupTable
        CANMsg
        Logger
        )
mbed_set_post_build(test-lookup-table)
