#
# tests/pywatch/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

set(PYWATCHPY ${CMAKE_CURRENT_SOURCE_DIR}/pywatch.py)
set(EZ430_BINDING_PYTHON_PATH ${CMAKE_BINARY_DIR}/binding/python)
configure_file(pywatch.in pywatch @ONLY)
