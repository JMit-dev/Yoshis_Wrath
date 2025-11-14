cmake_minimum_required(VERSION 3.15)
project(yoshis_wrath CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /DDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
endif()

# Raylib configuration
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

# JSON library configuration
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(external/json)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/raylib/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/json/include)

# Collect source files
file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Remove editor sources from game build
list(FILTER GAME_SOURCES EXCLUDE REGEX ".*editor.*")

# Main game executable
add_executable(yoshis_wrath ${GAME_SOURCES})
target_link_libraries(yoshis_wrath PRIVATE raylib nlohmann_json::nlohmann_json)

# Set assets path - use relative path for both dev and release
target_compile_definitions(yoshis_wrath PUBLIC
    ASSETS_PATH="./assets/")

# Copy assets to build directory (for development)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET yoshis_wrath POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:yoshis_wrath>/assets
        COMMENT "Copying assets to build directory"
    )
endif()
