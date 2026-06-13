cmake_minimum_required(VERSION 3.16)

# Executable name defaults to current folder name (e.g., quest/q1 -> q1)
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${TARGET_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

    # For single-config generators (Ninja/Makefiles), default to Debug
# so manual debugging has symbols and disabled optimizations by default.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Paths
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SRC_DIR "${ROOT_DIR}/src")

# (b) Auto-detect source files with globs
file(GLOB QUEST_CPP CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB SHARED_CPP CONFIGURE_DEPENDS
        "${SRC_DIR}/*.cpp"
)

# Build one executable from local quest/*.cpp + shared src/*.cpp
add_executable(main
        ${QUEST_CPP}
        ${SHARED_CPP}
)

# Debug-friendly flags in a portable way (MSVC, GCC, Clang).
target_compile_options(main PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od /Zi>
        $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-O0 -g>
)

# Emit linker debug information for MSVC in Debug builds.
target_link_options(main PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/DEBUG>
)

# (a) Include src headers so #include "stuff.h" works
target_include_directories(main
        PRIVATE
        "${SRC_DIR}"
)

set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Also handle multi-config generators (Visual Studio, Xcode)
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${config}" config_upper)
    set_target_properties(main PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endforeach()