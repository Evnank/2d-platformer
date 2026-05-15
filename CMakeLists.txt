cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE EMBEDDED_ASSET_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/levels/*")
list(APPEND EMBEDDED_ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/settings.txt")
list(SORT EMBEDDED_ASSET_FILES)

set(EMBEDDED_ASSETS_DIR "${CMAKE_BINARY_DIR}/generated")
set(EMBEDDED_ASSETS_MANIFEST "${EMBEDDED_ASSETS_DIR}/embedded_assets_manifest.txt")
set(EMBEDDED_ASSETS_CPP "${EMBEDDED_ASSETS_DIR}/embedded_assets.cpp")

file(MAKE_DIRECTORY "${EMBEDDED_ASSETS_DIR}")
file(WRITE "${EMBEDDED_ASSETS_MANIFEST}" "")
foreach(ASSET_FILE IN LISTS EMBEDDED_ASSET_FILES)
    file(RELATIVE_PATH ASSET_KEY "${CMAKE_CURRENT_SOURCE_DIR}" "${ASSET_FILE}")
    string(REPLACE "\\" "/" ASSET_KEY "${ASSET_KEY}")
    file(APPEND "${EMBEDDED_ASSETS_MANIFEST}" "${ASSET_FILE}|${ASSET_KEY}\n")
endforeach()

add_custom_command(
    OUTPUT "${EMBEDDED_ASSETS_CPP}"
    COMMAND "${CMAKE_COMMAND}"
        "-DOUTPUT=${EMBEDDED_ASSETS_CPP}"
        "-DMANIFEST=${EMBEDDED_ASSETS_MANIFEST}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_assets.cmake"
    DEPENDS
        ${EMBEDDED_ASSET_FILES}
        "${EMBEDDED_ASSETS_MANIFEST}"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_assets.cmake"
    VERBATIM)

add_executable(main
    src/main.cpp
    src/embedded_assets.hpp
    "${EMBEDDED_ASSETS_CPP}")



target_compile_features(main PRIVATE cxx_std_20)

target_include_directories(main PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src")


target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
