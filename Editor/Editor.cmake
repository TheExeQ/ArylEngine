################
#### EDITOR ####
################

# Editor target
set(TARGET_NAME "Editor")

# Add sources
set(EDITOR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add executable
add_executable(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Runtime)

AddSources(${TARGET_NAME} ${EDITOR_PATH})

# Add PCH
set(PCH_HEADER "${EDITOR_PATH}/edpch.h")
target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_HEADER})

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE "${EDITOR_PATH}")

# Add link dependencies
target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    "Aryl" 
    "Game"
)