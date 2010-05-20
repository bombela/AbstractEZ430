#
# tests/openserial/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(syncmode
	test.cpp
	)

target_link_libraries(syncmode ez430_watch)
