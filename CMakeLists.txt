cmake_minimum_required(VERSION 4.0.0)


project(Bomberman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(PROJECT_FOLDER "Game")
set(RESOURCE_FOLDER_NAME "Resources")

message("\nRoot CMakeList")


# Steamworks
set(STEAMWORKS_ROOT "${CMAKE_SOURCE_DIR}/3rdParty/steamworks")
set(STEAMWORKS_ENABLED ON)

# Enable Console
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ENABLE_CONSOLE ON)
else()
    set(ENABLE_CONSOLE OFF)
endif()


if(STEAMWORKS_ENABLED AND NOT EMSCRIPTEN AND EXISTS "${STEAMWORKS_ROOT}")
    set(STEAMWORKS_ENABLED ON)
else()
    set(STEAMWORKS_ENABLED OFF)
endif()


set(UPDATE_SUBMODULES ON)

if(UPDATE_SUBMODULES)
    execute_process(
        COMMAND git submodule update --remote --rebase
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMODULE_RESULT
    )

    if(NOT GIT_SUBMODULE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to update the submodules")
    endif()
endif()


# Add Steamworks
if(STEAMWORKS_ENABLED)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/steam")
    find_package(Steamworks QUIET)
    if(NOT Steamworks_FOUND)
        message("\n")
        message(WARNING "Steamworks package NOT found")
        set(STEAMWORKS_ENABLED FALSE)

    else()
        message("\n")
        message("Steamworks package Found")
        set(STEAMWORKS_ENABLED TRUE)

    endif()
endif()

add_subdirectory(BudgetArmsEngine)
add_subdirectory(Game)
