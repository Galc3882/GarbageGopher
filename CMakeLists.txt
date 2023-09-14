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

# Link the OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(GarbageGopher ${OpenCV_LIBS})

# Link the ONNX library
find_package(ONNX REQUIRED)
include_directories(${ONNX_INCLUDE_DIRS})
target_link_libraries(GarbageGopher ${ONNX_LIBS})

# Link the GTSAM library (for localization)
find_package(GTSAM REQUIRED)
include_directories(${GTSAM_INCLUDE_DIRS})
target_link_libraries(GarbageGopher ${GTSAM_LIBRARIES})

# Link the Torch library (for MLModel)
find_package(Torch REQUIRED)
target_link_libraries(GarbageGopher "${TORCH_LIBRARIES}")

# Link threads (for threading related functionality, e.g., UltrasonicSensor)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(GarbageGopher Threads::Threads)

# Enable verbose output
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Declare the dependency on Python 3.7 and the Jetson.GPIO library
SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
SET(GCC_COVERAGE_LINK_FLAGS    "-fPIC")

# Add the flags to the compiler
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")