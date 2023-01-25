project(GarbageGopher)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(GarbageGopher ${SOURCES})

# Declare the dependency on Python 3.7 and the Jetson.GPIO library
SET(GCC_COVERAGE_COMPILE_FLAGS "$(python3.7-config --cflags )")
SET(GCC_COVERAGE_LINK_FLAGS    "$(python3.7-config --ldflags) -fPIC -lJetsonGPIO -lpthread")

# Add the flags to the compiler
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")