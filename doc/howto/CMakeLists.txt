#
# doc/howto/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

project(howto)

find_package(LATEX)

set(HOWTOSRC ${howto_SOURCE_DIR}/howto.tex)
set(HOWTOPDF ${howto_BINARY_DIR}/howto.pdf)
set(HOWTOLOG ${howto_BINARY_DIR}/howto.log)

if(PDFLATEX_COMPILER)
	add_custom_command( 
		OUTPUT  ${HOWTOPDF}
		COMMAND ${PDFLATEX_COMPILER}
		ARGS    ${HOWTOSRC}
		DEPENDS ${HOWTOSRC}
		COMMENT "Building ${HOWTOPDF}..."
		)
	add_custom_command( 
		OUTPUT  ${HOWTOLOG}
		COMMAND ${PDFLATEX_COMPILER}
		ARGS    ${HOWTOSRC}
		DEPENDS ${HOWTOPDF}
		COMMENT "Re-building ${HOWTOPDF}..."
		)

	set(HOWTO_RELEASE TRUE CACHE BOOL "Building release version of howto documentation")
	if(HOWTO_RELEASE)
		add_custom_target(howto ALL DEPENDS ${HOWTOLOG})
	else()
		add_custom_target(howto ALL DEPENDS ${HOWTOPDF})
	endif()
else(PDFLATEX_COMPILER)
	message(WARNING ${PDFLATEX_COMPILER} compiler not found! Howto will be not generated)
endif(PDFLATEX_COMPILER)
