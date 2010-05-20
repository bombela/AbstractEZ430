#
# tests/openserial/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3 -D_GLIBCXX_DEBUG")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(syncmode
	test.cpp
	)

target_link_libraries(syncmode ez430_watch)
