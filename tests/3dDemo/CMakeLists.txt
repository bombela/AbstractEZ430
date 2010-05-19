#
# tests/3dDemo/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

set(DEMOPY ${CMAKE_CURRENT_SOURCE_DIR}/demo.py)
set(EZ430_BINDING_PYTHON_PATH ${CMAKE_BINARY_DIR}/binding/python)
configure_file(demo.in demo @ONLY)
