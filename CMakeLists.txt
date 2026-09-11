cmake_minimum_required(VERSION 4.0)
project(Simulation
        LANGUAGES CXX
        VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 26)

add_executable(${CMAKE_PROJECT_NAME}
        src/BodyUtil.cpp)

add_subdirectory(dependencies)
add_subdirectory(src)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -Wno-sign-conversion)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)


add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
