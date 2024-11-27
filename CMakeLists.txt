cmake_minimum_required(VERSION 3.21)

project(
    KeyLogicExpansion
    VERSION 1.0.0
    DESCRIPTION "A SKSE plugin which make key input smooth and modern."
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include(GNUInstallDirs)

# change this to your output mod folder
# AE Vanilla
# set(PLUGIN_OUTPUT "C:/Users/ACFOV/Documents/MO2/Skyrim/Steam/mods/KeyLogicOverhaul")
# MCO
set(PLUGIN_OUTPUT "C:/Users/ACFOV/Documents/MO2/Skyrim/Requiem/mods/KeyLogicOverhaul")
# BFCO
# set(PLUGIN_OUTPUT "C:/Users/ACFOV/Documents/MO2/Skyrim/BFCO/mods/KeyLogicOverhaul")

file(GLOB_RECURSE headers 
    "src/lib/*.h"
)
file(GLOB_RECURSE sources 
    "src/plugin.cpp"
    "src/lib/*.cpp"
)

# Setup your SKSE plugin as an SKSE plugin!
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} 
    AUTHOR "Acook1e"
    USE_ADDRESS_LIBRARY
    USE_SIGNATURE_SCANNING
    SOURCES ${headers} ${sources}
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/lib/PCH.h)

if(NOT DEFINED PLUGIN_OUTPUT)
    set(OUTPUT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
else()
    set(OUTPUT_FOLDER "${PLUGIN_OUTPUT}")
endif()

if(DEFINED OUTPUT_FOLDER)
    
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    # If you perform a "Debug" build, also copy .pdb file (for debug symbols)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()
