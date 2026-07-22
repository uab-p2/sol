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

file(GLOB SHARED_HEADERS CONFIGURE_DEPENDS
        "${SRC_DIR}/*.h"
)

# Build `main` from local quest/*.cpp except test.cpp + shared src/*.cpp
file(GLOB QUEST_MAIN_CPP CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
file (GLOB QUEST_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

list(REMOVE_ITEM QUEST_MAIN_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)
add_executable(${MAIN_TARGET}
        ${QUEST_MAIN_CPP}
        ${QUEST_HEADERS}
        ${SHARED_CPP}
        ${SHARED_HEADERS}
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
        ${QUEST_HEADERS}
        ${SHARED_CPP}
        ${SHARED_HEADERS}
)


function(enable_warnings_and_debug target_name)
    if (MSVC)
        target_compile_options(${target_name} PRIVATE
                /W4                             # high warning verbosity
                /permissive-                    # stricter standard conformance
                $<$<CONFIG:Debug>:/Zi>          # full debug info
                $<$<CONFIG:Debug>:/Od>          # disable optimizations
                $<$<CONFIG:Debug>:/RTC1>        # runtime checks (debug)
                $<$<CONFIG:Debug>:/sdl>         # extra runtime/security checks
        )
        target_link_options(${target_name} PRIVATE
                $<$<CONFIG:Debug>:/DEBUG>       # emit PDB debug symbols
        )

    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target_name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Woverloaded-virtual
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
                $<$<CONFIG:Debug>:-g3>          # max practical debug info
                $<$<CONFIG:Debug>:-O0>          # disable optimizations
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target_name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Woverloaded-virtual
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast
                $<$<CONFIG:Debug>:-g3>          # richer debug symbols
                $<$<CONFIG:Debug>:-O0>          # disable optimizations
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
                $<$<CONFIG:Debug>:-fno-inline>  # easier stepping in debugger
        )
    endif ()
endfunction()
foreach (target ${MAIN_TARGET} ${TEST_TARGET})
    # Warning verbose and Debug-friendly flags (MSVC, GCC, Clang).
    enable_warnings_and_debug(${target})

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

