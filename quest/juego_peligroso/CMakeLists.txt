cmake_minimum_required(VERSION 3.16)

# Executable name defaults to current folder name (e.g., quest/q1 -> q1)
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${TARGET_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Off by default: a normal student build never needs gcov/lcov
# instrumentation. Gradescope turns this on for a dedicated,
# separate build (see luna.documentation.CoverageAnalyzer) used only to
# measure test coverage, never for the functional/valgrind test run.
option(ENABLE_COVERAGE "Build ${TEST_TARGET} with gcov/lcov instrumentation" OFF)

# Group internal/vendored targets (Catch2, common_obj) into an IDE
# subfolder instead of listing them flat alongside main/test - VS's
# CMake Targets View and Xcode's scheme list both honor FOLDER.
# Ignored by generators that don't support it (Ninja, Makefiles).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Catch2 3 QUIET CONFIG)
if (NOT Catch2_FOUND)
    message(STATUS "Catch2 was not found locally. Falling back to FetchContent. Consider installing Catch2 system-wide to speed up configuration.")
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            # Use an archive URL instead of git clone to avoid safe.directory
            # ownership failures on shared folders (common on Windows/VM setups).
            URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.tar.gz
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    get_property(_catch2_targets DIRECTORY "${catch2_SOURCE_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
    set_target_properties(${_catch2_targets} PROPERTIES FOLDER "ThirdParty/Catch2")
endif ()

# Paths
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SRC_DIR "${ROOT_DIR}/src")

# When included from a top-level CMake project, avoid global target name collisions.
if (DEFINED QUEST_TARGET_PREFIX AND NOT QUEST_TARGET_PREFIX STREQUAL "")
    set(MAIN_TARGET "${QUEST_TARGET_PREFIX}_main")
    set(TEST_TARGET "${QUEST_TARGET_PREFIX}_test")
    set(COMMON_OBJ "${QUEST_TARGET_PREFIX}_common_obj")
else ()
    set(MAIN_TARGET main)
    set(TEST_TARGET test)
    set(COMMON_OBJ common_obj)
endif ()

file(GLOB SHARED_CPP CONFIGURE_DEPENDS
        "${SRC_DIR}/*.cpp"
)

# .hpp as well as .h: a template's methods have to be defined in a
# header, so a quest can be solved with a header-only .hpp and no .cpp
# of its own at all.
file(GLOB SHARED_HEADERS CONFIGURE_DEPENDS
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.hpp"
)

file(GLOB QUEST_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# Local quest/*.cpp other than main.cpp/test.cpp themselves (e.g. a quest's
# own helper .cpp files) are compiled into both main and test. Together with
# shared src/*.cpp, that source set used to be listed separately in each
# add_executable() below, so every file in it was compiled twice - once per
# target. Compiling it once into an OBJECT library and linking that into
# both executables halves that redundant work.
file(GLOB ALL_QUEST_CPP CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
set(COMMON_QUEST_CPP ${ALL_QUEST_CPP})
list(REMOVE_ITEM COMMON_QUEST_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)

# Debug-friendly flags in a portable way (MSVC, GCC, Clang).
# Helper: apply strong warnings and reasonable debug flags cross-platform.
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
                $<$<CONFIG:Debug>:-fno-inline>  # easier debugger stepping
        )
    endif()
endfunction()

# Plain (non-instrumented) common objects, used by main and, when coverage
# isn't enabled, by test as well.
add_library(${COMMON_OBJ} OBJECT
        ${SHARED_CPP}
        ${COMMON_QUEST_CPP}
        ${SHARED_HEADERS}
        ${QUEST_HEADERS}
)
enable_warnings_and_debug(${COMMON_OBJ})
target_include_directories(${COMMON_OBJ} PRIVATE "${SRC_DIR}")
set_target_properties(${COMMON_OBJ} PROPERTIES FOLDER "Internal")

# Headers aren't compiled (CMake only builds recognized source
# extensions), so re-listing them here costs nothing at build time -
# it's just so IDEs that show sources per-target (Visual Studio's
# per-vcxproj view, CLion) still find .h/.hpp files (including a
# quest's header-only .hpp, e.g. metamorfosis) under main/test too,
# not only under the shared ${COMMON_OBJ} object library.
add_executable(${MAIN_TARGET}
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        ${SHARED_HEADERS}
        ${QUEST_HEADERS}
)
target_link_libraries(${MAIN_TARGET} PRIVATE ${COMMON_OBJ})

# A gcov/lcov coverage build only ever builds the `test` target (see
# luna.coverage.CoverageAnalyzer), so it's safe to instrument the common
# objects too when ENABLE_COVERAGE is on - but they must be a *separate*
# OBJECT library from ${COMMON_OBJ}: the same object files can't be both
# instrumented and not, and main must stay uninstrumented, matching the
# instrumentation split already applied below to ${TEST_TARGET} itself.
if (ENABLE_COVERAGE)
    set(TEST_OBJ "${COMMON_OBJ}_cov")
    add_library(${TEST_OBJ} OBJECT
            ${SHARED_CPP}
            ${COMMON_QUEST_CPP}
            ${SHARED_HEADERS}
            ${QUEST_HEADERS}
    )
    enable_warnings_and_debug(${TEST_OBJ})
    target_include_directories(${TEST_OBJ} PRIVATE "${SRC_DIR}")
    target_compile_options(${TEST_OBJ} PRIVATE --coverage)
    set_target_properties(${TEST_OBJ} PROPERTIES FOLDER "Internal")
else ()
    set(TEST_OBJ ${COMMON_OBJ})
endif ()

add_executable(${TEST_TARGET}
        "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
        ${SHARED_HEADERS}
        ${QUEST_HEADERS}
)
target_link_libraries(${TEST_TARGET} PRIVATE ${TEST_OBJ})

# Catch2 linking for test target.
# Prefer already-available v3 targets (either from a local package or FetchContent).
if (TARGET Catch2::Catch2WithMain)
    target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2WithMain)
elseif (TARGET Catch2::Catch2)
    target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2)
else ()
    # Final fallback for environments that only provide a packaged Catch2 v2.
    find_package(Catch2 2 REQUIRED CONFIG)
    if (TARGET Catch2::Catch2WithMain)
        target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2WithMain)
    else ()
        target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2)
    endif ()
endif ()

foreach (target ${MAIN_TARGET} ${TEST_TARGET})
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

# Applied last so it overrides the RUNTIME_OUTPUT_DIRECTORY(_<CONFIG>)
# defaults set above: an instrumented ${TEST_TARGET} must never land at
# the same path as the plain one those defaults point every target at,
# or it would silently replace the binary the functional/valgrind test
# run uses.
if (ENABLE_COVERAGE)
    target_compile_options(${TEST_TARGET} PRIVATE --coverage)
    target_link_options(${TEST_TARGET} PRIVATE --coverage)
    set_target_properties(${TEST_TARGET} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
    foreach (config Debug Release RelWithDebInfo MinSizeRel)
        string(TOUPPER "${config}" config_upper)
        set_target_properties(${TEST_TARGET} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_CURRENT_BINARY_DIR}"
        )
    endforeach ()
endif ()