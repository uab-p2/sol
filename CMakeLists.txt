# Master CMakeLists.txt that lets edit all quest targets.
# Students are encouraged to open each quest's individual CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

project(sol LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep a simple switch in case you want a root project without building quests.
option(SOL_BUILD_QUESTS "Build all quest subprojects" ON)

if (SOL_BUILD_QUESTS)
    file(GLOB QUEST_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/quest" "${CMAKE_CURRENT_SOURCE_DIR}/quest/*")

    foreach (quest_rel_dir IN LISTS QUEST_DIRS)
        set(quest_abs_dir "${CMAKE_CURRENT_SOURCE_DIR}/quest/${quest_rel_dir}")

        if (IS_DIRECTORY "${quest_abs_dir}" AND EXISTS "${quest_abs_dir}/CMakeLists.txt")
            # Prefix target names so each quest can still define main/test locally.
            set(QUEST_TARGET_PREFIX "${quest_rel_dir}")
            add_subdirectory("${quest_abs_dir}" "${CMAKE_BINARY_DIR}/quest/${quest_rel_dir}")
            unset(QUEST_TARGET_PREFIX)
        endif ()
    endforeach ()
endif ()

