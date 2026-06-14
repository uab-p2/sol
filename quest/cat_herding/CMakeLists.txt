cmake_minimum_required(VERSION 3.16)

# Executable name defaults to current folder name (e.g., quest/q1 -> q1)
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${TARGET_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Paths
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SRC_DIR "${ROOT_DIR}/src")

# When included from a top-level CMake project, avoid global target name collisions.
if (DEFINED QUEST_TARGET_PREFIX AND NOT QUEST_TARGET_PREFIX STREQUAL "")
    set(MAIN_TARGET "${QUEST_TARGET_PREFIX}_main")
    set(TEST_TARGET "${QUEST_TARGET_PREFIX}_test")
else ()
    set(MAIN_TARGET main)
    set(TEST_TARGET test)
endif ()

file(GLOB SHARED_CPP CONFIGURE_DEPENDS
        "${SRC_DIR}/*.cpp"
)

# Build `main` from local quest/*.cpp except test.cpp + shared src/*.cpp
file(GLOB QUEST_MAIN_CPP CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(REMOVE_ITEM QUEST_MAIN_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)
add_executable(${MAIN_TARGET}
        ${QUEST_MAIN_CPP}
        ${SHARED_CPP}
)

# Build `test` from local quest/*.cpp except main.cpp + shared src/*.cpp
file(GLOB QUEST_TEST_CPP CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(REMOVE_ITEM QUEST_TEST_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
add_executable(${TEST_TARGET}
        ${QUEST_TEST_CPP}
        ${SHARED_CPP}
)

# Debug-friendly flags in a portable way (MSVC, GCC, Clang).
foreach (target ${MAIN_TARGET} ${TEST_TARGET})
    target_compile_options(${target} PRIVATE
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od /Zi>
            $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-O0 -g>
    )

    # Emit linker debug information for MSVC in Debug builds.
    target_link_options(${target} PRIVATE
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/DEBUG>
    )

    # Include src headers so #include "stuff.h" works
    target_include_directories(${target}
            PRIVATE
            "${SRC_DIR}"
    )

    set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endforeach ()

# For single-config generators (Ninja/Makefiles), default to Debug
# so manual debugging has symbols and disabled optimizations by default.
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()
# Also handle multi-config generators (VS, XC, CL)
foreach (config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${config}" config_upper)
    foreach (target ${MAIN_TARGET} ${TEST_TARGET})
        set_target_properties(${target} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach ()
endforeach ()
