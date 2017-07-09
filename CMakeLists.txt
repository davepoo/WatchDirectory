cmake_minimum_required(VERSION 3.7)

project( WatchDirectory )

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set  (SRC
		"WatchDirectory.h"
		"main.cpp")
	
source_group("src" FILES ${SRC})

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_executable (WatchDirectory ${SRC})