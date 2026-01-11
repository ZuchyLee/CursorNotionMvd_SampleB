cmake_minimum_required(VERSION 3.20)

project(ProjectB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common module (git submodule)
add_subdirectory(CursorNotionMvd_CommonModule)

add_executable(ProjectB_AutomotiveDemo
    src/main.cpp
)

# The common module currently uses directory-scope include_directories(include),
# which does not reliably propagate to this target. We explicitly add the public
# include directory here to keep the integration stable.
target_include_directories(ProjectB_AutomotiveDemo
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/CursorNotionMvd_CommonModule/include
)

target_link_libraries(ProjectB_AutomotiveDemo
    PRIVATE
        AutomotiveCore
)

