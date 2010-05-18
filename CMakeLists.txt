#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.4)

project(AbstractEZ430 CXX)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3 -D_GLIBCXX_DEBUG")
endif()

set(Boost_USE_STATIC_LIBS   OFF)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_INCLUDEDIR "/usr/local/include")
set(Boost_ADDITIONAL_VERSIONS 1.42 1.41 1.40 1.39 1.38)

add_definitions(-DBOOST_ALL_NO_LIB)
add_subdirectory(tests)
add_subdirectory(lib)
add_subdirectory(binding)
