#
# tests/abspos/CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

set(ABSPOSPY ${CMAKE_CURRENT_SOURCE_DIR}/abspos.py)
set(EZ430_BINDING_PYTHON_PATH ${CMAKE_BINARY_DIR}/binding/python)
configure_file(abspos.in abspos @ONLY)
