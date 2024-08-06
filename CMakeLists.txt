# Set which version of cmake we need, will throw an error if this check fails.
cmake_minimum_required(VERSION 3.16...3.23)

project(BasicCServer)

include_directories( "src" )
include_directories( "util" )

file (GLOB source_files CONFIGURE_DEPENDS
		"*.cpp"
		"*.h"
			"Src/*.cpp"
			"Src/*.h"
				#"src/CE303/*.cpp" See the GLOB_RECURSE
				#"src/CE303/*.h"
					#"src/CE303/Client/*.cpp"
					#"src/CE303/Client/*.h"
			"Util/Datastructs/*.cpp"
			"Util/Datastructs/*.h"
				"Util/Datastructs/SocketType/*.cpp"
				"Util/Datastructs/SocketType/*.h"
)

file (GLOB_RECURSE ce303_files CMAKE_CONFIGURE_DEPENDS
		"Src/CE303/*.cpp"
		"Src/CE303/*.h"
)

# Add source to this project's library.
add_executable(${PROJECT_NAME} ${source_files} ${ce303_files})

# Set which version of C++ we want to use
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()