# CMake file for primary executable

# Generate version source files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)

# Create binary executable
add_executable(${PROJECT_NAME}
    minesweeper.cpp
    Minefield.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

# Add install command
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "/usr/local/bin")
