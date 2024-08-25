project(examplelib VERSION 0.1 LANGUAGES CXX)
add_library(examplelib SHARED examplelib.cpp)

add_compile_definitions(EXAMPLELIB_EXPORTS)
target_include_directories(examplelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})