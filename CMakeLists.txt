cmake_minimum_required(VERSION 3.16)
project(tinyml_sensor_anomaly_detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(${PROJECT_NAME}_core
  src/ProjectRuntime.cpp
)
target_include_directories(${PROJECT_NAME}_core PUBLIC include)
target_compile_options(${PROJECT_NAME}_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME}
  src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_core)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_executable(project_runtime_tests
  tests/ProjectRuntimeTests.cpp
)
target_link_libraries(project_runtime_tests PRIVATE ${PROJECT_NAME}_core)
target_compile_options(project_runtime_tests PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME project_runtime_tests COMMAND project_runtime_tests)
