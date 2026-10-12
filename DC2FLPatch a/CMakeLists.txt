cmake_minimum_required(VERSION 3.15)
project(DC2FLPatch)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Error: Building for x64, but x86 expected. Use x86 Native Tools Command Prompt.")
endif()

message(STATUS "Configuring build for Da Capo II Fall in Love...")

# Detours library
set(DETOURS_DIR "${CMAKE_SOURCE_DIR}/Detours/src")
include_directories(${DETOURS_DIR})

add_library(detours STATIC
    ${DETOURS_DIR}/detours.cpp
    ${DETOURS_DIR}/modules.cpp
    ${DETOURS_DIR}/disasm.cpp
    ${DETOURS_DIR}/image.cpp
    ${DETOURS_DIR}/creatwth.cpp
    ${DETOURS_DIR}/disolx86.cpp
)

# ============================================================================
# DC2FLPatch.dll - The actual patch (injected by launcher)
# ============================================================================
add_library(DC2FLPatch SHARED
    src/dc2flpatch.cpp
)

target_link_libraries(DC2FLPatch PRIVATE 
    detours
    gdi32
    user32
    advapi32
    comdlg32
)

set_target_properties(DC2FLPatch PROPERTIES
    LINK_FLAGS "/DYNAMICBASE:NO"
    OUTPUT_NAME "DC2FLPatch"
)

target_compile_definitions(DC2FLPatch PRIVATE GAME_DC2FL)

# ============================================================================
# DC2FLLauncher.exe - Starts the game with Japanese locale and injects DC2FLPatch.dll
# ============================================================================
add_executable(DC2FLLauncher WIN32
    src/launcher.cpp
    src/launcher.rc
)

target_include_directories(DC2FLLauncher PRIVATE src)

target_link_libraries(DC2FLLauncher PRIVATE
    user32
    advapi32
)

set_target_properties(DC2FLLauncher PROPERTIES OUTPUT_NAME "DC2FLLauncher")
target_compile_definitions(DC2FLLauncher PRIVATE GAME_DC2FL)
