cmake_minimum_required(VERSION 3.9)

project(GarbageGopher)
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(GarbageGopher ${SOURCES})

# Link the Python library
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(GarbageGopher ${PYTHON_LIBRARIES})

# Link the Jetson.GPIO library
find_package(JetsonGPIO)
target_link_libraries(GarbageGopher JetsonGPIO::JetsonGPIO)

# # Enable verbose output
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Declare the dependency on Python 3.7 and the Jetson.GPIO library
SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
SET(GCC_COVERAGE_LINK_FLAGS    "-fPIC")

# Add the flags to the compiler
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")