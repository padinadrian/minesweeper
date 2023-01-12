# CMake file for unit test executable

# Include settings
include_directories("/usr/include")

# List unit test source files
add_executable(${PROJECT_NAME}_tests
    Minefield_tests.cpp
)

# Add dependent libraries
target_link_libraries(${PROJECT_NAME}_tests
    gtest
    gtest_main
    pthread
)
