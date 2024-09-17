cmake_minimum_required(VERSION 3.12)

project(renderer)

add_subdirectory(external/glfw)

file(GLOB_RECURSE SOURCES 
    src/*.cpp 
    external/glad/*.c
)

include_directories(
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/include/core"
    "${PROJECT_SOURCE_DIR}/include/external"
    "${PROJECT_SOURCE_DIR}/include/rendering"
)

add_executable(${PROJECT_NAME} ${SOURCES})

#On windows you only need to link glfw
target_link_libraries(${PROJECT_NAME} glfw -pthread -ldl -lm)