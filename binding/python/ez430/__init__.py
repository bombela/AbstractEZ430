#!/usr/bin/env python
#
# binding/python/__init__.py
# Copyright (C) 2010 Francois-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

from _ez430 import *

def Motion_str(self):
	if self.haveAcc:
		return "button=%s x=%i y=%i z=%i" % (self.button, self.x, self.y, self.z)
	else:
		return "button=%s (no accel data)" % (self.button)

Motion.__str__ = Motion_str
