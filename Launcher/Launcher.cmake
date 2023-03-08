##################
#### LAUNCHER ####
##################

# Editor target
set(TARGET_NAME "Launcher")

# Add sources
set(LAUNCHER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add executable
add_executable(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Runtime)

AddSources(${TARGET_NAME} ${LAUNCHER_PATH})

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE "${LAUNCHER_PATH}")

# Add link dependencies
target_link_libraries(${TARGET_NAME} PRIVATE "Aryl")
target_link_libraries(${TARGET_NAME} PRIVATE "Game")