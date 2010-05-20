#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

find_package(Boost 1.38 REQUIRED system thread date_time)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(PLATEFORM_SRCS
	probeports.cpp
	)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(PLATEFORM linux)
else()
	message(WARNING "Plateform not supported, using generic placeholder code")
	set(PLATEFORM generic)
endif()

set(PLATEFORM_SRCS_PATH)
foreach(src ${PLATEFORM_SRCS})
	set(PLATEFORM_SRCS_PATH ${PLATEFORM_SRCS_PATH} "${PLATEFORM}/${src}")
endforeach()

add_library(ez430_watch SHARED
	ap.cpp
	ap_service.cpp
	watch.cpp
	${PLATEFORM_SRCS_PATH}
	)

target_link_libraries(ez430_watch ${Boost_LIBRARIES})
