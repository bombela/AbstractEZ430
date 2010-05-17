#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.4)


set(Boost_USE_STATIC_LIBS   OFF)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_INCLUDEDIR "/usr/local/include")
set(Boost_ADDITIONAL_VERSIONS 1.42 1.41 1.40 1.39 1.38)
add_definitions(-DBOOST_ALL_NO_LIB)
find_package(Boost 1.38 REQUIRED system)
include_directories(${Boost_INCLUDE_DIRS})


include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ez430_watch SHARED
	ap.cpp
	)

target_link_libraries(ez430_watch ${Boost_LIBRARIES})
