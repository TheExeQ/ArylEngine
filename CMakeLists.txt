cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)

project(Aryl)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Editor")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Engine target
set(TARGET_NAME ${PROJECT_NAME})

# add the sources
set(TARGET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")
set(ARYL_PATH "${TARGET_PATH}")

file(GLOB_RECURSE TARGET_SOURCE_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.cpp")
file(GLOB_RECURSE TARGET_HEADER_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.h")

source_group(TREE ${TARGET_PATH} FILES ${TARGET_SOURCE_FILES})
source_group(TREE ${TARGET_PATH} FILES ${TARGET_HEADER_FILES})

# add the library
add_library(${TARGET_NAME} ${TARGET_SOURCE_FILES} ${TARGET_HEADER_FILES})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Core)

# add include directories
target_include_directories(${TARGET_NAME} PRIVATE "${TARGET_PATH}/src")

# add dependencies
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
    glm::glm
    glfw
)

# Editor target
set(TARGET_NAME "Editor")

# add the sources
set(TARGET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")
set(EDITOR_PATH "${TARGET_PATH}")

file(GLOB_RECURSE TARGET_SOURCE_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.cpp")
file(GLOB_RECURSE TARGET_HEADER_FILES CONFIGURE_DEPENDS "${TARGET_PATH}/*.h")

source_group(TREE ${TARGET_PATH} FILES ${TARGET_SOURCE_FILES})
source_group(TREE ${TARGET_PATH} FILES ${TARGET_HEADER_FILES})

# add the executable
add_executable(${TARGET_NAME} ${TARGET_SOURCE_FILES} ${TARGET_HEADER_FILES})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Editor)

# add include directories
target_include_directories(${TARGET_NAME} PRIVATE "${TARGET_PATH}/src")
target_include_directories(${TARGET_NAME} PRIVATE "${ARYL_PATH}/src")

# add dependencies
target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME})