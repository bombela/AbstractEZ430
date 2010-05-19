#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.4)

find_package(Boost 1.38 REQUIRED python)
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonLibs REQUIRED ALL)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ez430_watch_python MODULE
	ap.cpp
	)

target_link_libraries(ez430_watch_python
	ez430_watch
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES}
	)

configure_file(__init__.py __init__.py COPYONLY)

set_target_properties(ez430_watch_python PROPERTIES OUTPUT_NAME "ez430")
set_target_properties(ez430_watch_python PROPERTIES PREFIX "_")
