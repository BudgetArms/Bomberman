cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("\n")
message("${PROJECT_NAME} CMake")

if(EMSCRIPTEN)
    set(PROJECT_NAME ${PROJECT_NAME}_web)
    set(TARGET_PARAM)
else()
    set(PROJECT_NAME ${PROJECT_NAME})

    if(ENABLE_CONSOLE)
        set(TARGET_PARAM WIN32)
    else()
        set(TARGET_PARAM)
    endif()

endif()


add_executable(${PROJECT_NAME} ${TARGET_PARAM}
    "Main.cpp"

    "Base/CommonManagerVariables.hpp"
    "Base/GameMode.hpp"

    "Components/BombermanComponent.cpp"
    "Components/HitboxComponent.cpp"
    "Components/LifeComponent.cpp"
    "Components/LifeDisplayComponent.cpp"
    "Components/ScoreComponent.cpp"
    "Components/ScoreDisplayComponent.cpp"

    "Managers/CollisionManager.cpp"
)


# Compile at max warning level + treat warnings as errors
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


if(EMSCRIPTEN)
    target_link_options(${PROJECT_NAME} PRIVATE
        "-sEXPORTED_FUNCTIONS=[_main,_malloc,_free]"
        "-sEXPORTED_RUNTIME_METHODS=[HEAPF32,ccall,cwrap]"
    )
endif()

if(STEAMWORKS_ENABLED)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "STEAMWORKS_ENABLED"
    )
endif()

target_include_directories(${ENGINE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})


if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH \
        --preload-file \"${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}@/\"")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake")

    # Additional settings for Emscripten build
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/web"
        "${CMAKE_BINARY_DIR}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()


if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}"
        "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/${RESOURCE_FOLDER_NAME}"
    )
endif()


message("\n")

