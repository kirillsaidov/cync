cmake_minimum_required(VERSION 3.5)

# project name init
project(
    cync
    VERSION 1.0.1
    DESCRIPTION "Syncing files accross directories and devices."
    LANGUAGES C
)

# setting default build type
set(CMAKE_C_STANDARD 11)
set(default_buid_type "Release")
set(CMAKE_C_FLAGS "-DNDEBUG -Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# finding all headers and sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# add subproject
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/vita)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/inc/)
include_directories(${PROJECT_SOURCE_DIR}/third_party/vita/inc/)

# add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# linking 
target_link_libraries(${PROJECT_NAME} vita)



