#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.4)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ez430_watch SHARED
	ap.cpp
	)
