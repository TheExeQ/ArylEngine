##############
#### GAME ####
##############

# Game target
set(TARGET_NAME "Game")

# Add sources
set(GAME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add library
add_library(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Game)

AddSources(${TARGET_NAME} ${GAME_PATH})

# Add PCH
set(PCH_HEADER "${GAME_PATH}/gepch.h")
target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_HEADER})

# Include directories
target_include_directories(${TARGET_NAME} PUBLIC "${GAME_PATH}")

# Add link dependencies
target_link_libraries(${TARGET_NAME} PRIVATE "Aryl")