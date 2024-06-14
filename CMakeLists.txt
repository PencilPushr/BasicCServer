# Set which version of cmake we need, will throw an error if this check fails.
cmake_minimum_required(VERSION 3.16...3.23)

project(BasicCServer)

include_directories( "src" )

file (GLOB source_files CONFIGURE_DEPENDS
		"*.cpp"
		"*.h"
		"src/*.cpp"
		"src/*.h"
)

# Add source to this project's library.
add_executable(${PROJECT_NAME} ${source_files})

# Set which version of C++ we want to use
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()