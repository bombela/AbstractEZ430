#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.4)

find_package(Boost 1.38 REQUIRED system thread date_time)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ez430_watch SHARED
	ap.cpp
	ap_service.cpp
	watch.cpp
	)

target_link_libraries(ez430_watch ${Boost_LIBRARIES})
