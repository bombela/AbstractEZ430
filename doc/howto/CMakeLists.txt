#
# doc/howto/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

project(howto)

find_package(LATEX)

set(HOWTOSRC ${howto_SOURCE_DIR}/howto.tex)
set(HOWTOPDF ${howto_BINARY_DIR}/howto.pdf)

if(PDFLATEX_COMPILER)
	add_custom_command( 
		OUTPUT  ${HOWTOPDF}
		COMMAND ${PDFLATEX_COMPILER}
		ARGS    ${HOWTOSRC}
		DEPENDS ${HOWTOSRC}
		COMMENT "Building ${HOWTOPDF}..."
		)
else()
	message(WARNING ${PDFLATEX_COMPILER} compiler not found! Howto will be not generated)
endif()

add_custom_target(howto ALL echo
	DEPENDS ${HOWTOPDF}
	)
