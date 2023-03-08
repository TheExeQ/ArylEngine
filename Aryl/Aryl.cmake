################
#### ENGINE ####
################

# Engine target
set(TARGET_NAME "Aryl")

# Add sources
set(ARYL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}")

# Add library
add_library(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Core)

AddSources(${TARGET_NAME} ${ARYL_PATH})
target_compile_definitions(${TARGET_NAME} 
PUBLIC 
GLFW_INCLUDE_NONE
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
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
    imgui::imgui
    glad::glad
    glfw
    glm
    EnTT::EnTT
    spdlog::spdlog spdlog::spdlog_header_only
    asio::asio
)