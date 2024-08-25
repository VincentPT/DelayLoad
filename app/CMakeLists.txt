cmake_minimum_required(VERSION 3.16)

project(DelayLoadExample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(UNICODE)

set (DELAYLOADEXAMPLE_LINK_FLAGS "/DELAYLOAD:examplelib.dll")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${DELAYLOADEXAMPLE_LINK_FLAGS}")
add_executable(${PROJECT_NAME} main.cpp DelayLoadExample.rc)
target_link_libraries(${PROJECT_NAME} examplelib)