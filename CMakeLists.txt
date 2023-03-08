cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)

function(AddSources TARGET_NAME TARGET_PATH)
    file(GLOB_RECURSE TARGET_SOURCE_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.cpp")
    file(GLOB_RECURSE TARGET_HEADER_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.h")

    source_group(TREE ${TARGET_PATH} FILES ${TARGET_SOURCE_FILES})
    source_group(TREE ${TARGET_PATH} FILES ${TARGET_HEADER_FILES})

    target_sources(${TARGET_NAME} PRIVATE ${TARGET_SOURCE_FILES} ${TARGET_HEADER_FILES})
endfunction()

#################
#### PROJECT ####
#################

project(Aryl)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Editor")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add projects
include(Aryl/Aryl.cmake)
include(Game/Game.cmake)
include(Editor/Editor.cmake)
include(Launcher/Launcher.cmake)
