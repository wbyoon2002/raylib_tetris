cmake_minimum_required(VERSION 3.30.5)

project(my_raylib_game CXX)

# Set policy to allow FetchContent_Populate for compatibility
if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adding dependencies with FetchContent
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Configure Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

# Configure Socket.IO Client C++
FetchContent_Declare(
        socketio-client-cpp
        GIT_REPOSITORY "https://github.com/socketio/socket.io-client-cpp.git"
        GIT_TAG "master"
        GIT_SUBMODULES_RECURSE ON
        GIT_PROGRESS TRUE
)

# Configure socket.io-client-cpp build options before fetching
set(SIOCLIENT_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

# Get properties and apply patch manually
FetchContent_GetProperties(socketio-client-cpp)
if(NOT socketio-client-cpp_POPULATED)
    message(STATUS "Fetching socket.io-client-cpp...")
    FetchContent_Populate(socketio-client-cpp)
    
    # Apply the bug fix manually
    set(SIO_PACKET_FILE "${socketio-client-cpp_SOURCE_DIR}/src/internal/sio_packet.cpp")
    if(EXISTS "${SIO_PACKET_FILE}")
        file(READ "${SIO_PACKET_FILE}" PACKET_CONTENT)
        if(NOT PACKET_CONTENT MATCHES "#include <algorithm>")
            message(STATUS "Applying bug fix: adding #include <algorithm> to sio_packet.cpp")
            string(REGEX REPLACE "(#include \"sio_packet\\.h\")" "\\1\n#include <algorithm>" PATCHED_CONTENT "${PACKET_CONTENT}")
            file(WRITE "${SIO_PACKET_FILE}" "${PATCHED_CONTENT}")
            message(STATUS "Bug fix applied successfully")
        else()
            message(STATUS "Bug fix already applied")
        endif()
    endif()
    
    add_subdirectory(${socketio-client-cpp_SOURCE_DIR} ${socketio-client-cpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Make raylib available
FetchContent_MakeAvailable(raylib)

# include lib folder
include_directories(${PROJECT_SOURCE_DIR}/lib)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Link libraries with cross-platform socket library handling
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib sioclient ws2_32 wsock32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib sioclient)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets")
