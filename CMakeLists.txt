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

################
#### PROJECT ####
################

project(Aryl)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Editor")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

################
#### ENGINE ####
################

# Engine target
set(TARGET_NAME ${PROJECT_NAME})

# Add sources
set(ARYL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add library
add_library(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Core)

AddSources(${TARGET_NAME} ${ARYL_PATH})
target_compile_definitions(${TARGET_NAME} 
PUBLIC 
$<$<CONFIG:Debug>:YL_DEBUG>
$<$<CONFIG:Debug>:YL_ENABLE_ASSERTS>
$<$<CONFIG:RelWithDebInfo>:YL_RELDEB>
$<$<CONFIG:Release>:YL_RELEASE>
$<$<CONFIG:MinSizeRel>:YL_RELEASE>)

# Add PCH
set(PCH_HEADER "${ARYL_PATH}/ylpch.h")
target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_HEADER})

# Include directories
target_include_directories(${TARGET_NAME} PUBLIC "${ARYL_PATH}")

# Add link dependencies
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
    glad::glad
    glfw
    glm
    spdlog::spdlog spdlog::spdlog_header_only
)

################
#### EDITOR ####
################

# Editor target
set(TARGET_NAME "Editor")

# Add the sources
set(EDITOR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add the executable
add_executable(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Editor)

AddSources(${TARGET_NAME} ${EDITOR_PATH})

# Add PCH
set(PCH_HEADER "${EDITOR_PATH}/edpch.h")
target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_HEADER})

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE "${EDITOR_PATH}")

# Add link dependencies
target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME})