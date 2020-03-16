cmake_minimum_required (VERSION 2.8)
project(js-interpreter)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "GCC" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")
endif()

include_directories (include)
aux_source_directory (src JS_INTERPRETER_SRC)
add_library (jsilib "${JS_INTERPRETER_SRC}")
add_executable (${PROJECT_NAME} driver/jsi.cpp)
target_link_libraries (${PROJECT_NAME} jsilib)