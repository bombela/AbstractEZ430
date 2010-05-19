#!/usr/bin/env python
#
# tests/abspos/abspos.py
# Copyright (C) 2010 Francois-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

import ez430
import Gnuplot, Gnuplot.funcutils
from numpy import *

ap = ez430.AccessPoint()

ap.open("/dev/ttyACM0")

print "%x" % (ap.getProductId())

print "Current radio state: " + str(ap.getRadioState())

if ap.getRadioState() == ez430.RadioState.STOPPED:
	print "Starting radio..."
	ap.startRadio()

watch = ez430.Watch(ap.getService())
watch.setSmooth(0.80)

class MinMax:
	def __init__(self):
		self.min = 0
		self.max = 0

	def update(self, value):
		if value < self.min:
			self.min = value
		elif value > self.max:
			self.max = value

	def __str__(self):
		return "min=%i max=%i" % (self.min, self.max)

class ChangeSignDetector:
	def __init__(self):
		self.sign = 0

	def update(self, value):
		sign = 1
		if value < 0:
			sign = -1
		r = False
		if self.sign != 0 and sign != self.sign:
			r = True
		self.sign = sign
		return r

class ChangeSignCounter:
	def __init__(self):
		self.changeSignDetector = ChangeSignDetector()
		self.cnt = 0

	def update(self, value):
		if self.changeSignDetector.update(value):
			self.cnt += 1

	def __str__(self):
		return "cnt=%i" % (self.cnt)

class RotationDetector:
	def __init__(self):
		self.changeSignCounter = ChangeSignCounter()

	def getCnt(self):
		return self.changeSignCounter.cnt
	cnt = property(getCnt)

	def update(self, value):
		if abs(value) > 20:
			self.changeSignCounter.update(value)
	
	def __str__(self):
		return "cnt=%i" % (self.cnt)

class Calibration:
	def __init__(self):
		self.x = MinMax()
		self.y = MinMax()
		self.z = MinMax()

	def run(self):
		print "Calibrating... please move the watch around all axis two times..."

		rotX = RotationDetector()
		rotY = RotationDetector()
		rotZ = RotationDetector()
		nbrot = 2

		try:
			while rotX.cnt < nbrot or rotY.cnt < nbrot or rotZ.cnt < nbrot:
				motion = watch.getMotion()
				print motion

				self.x.update(motion.x)
				self.y.update(motion.y)
				self.z.update(motion.z)

				rotX.update(motion.x)
				rotY.update(motion.y)
				rotZ.update(motion.z)

				print "rx=%s ry=%s rz=%s" % (rotX, rotY, rotZ)

		except RuntimeError:
			print "Link timeout... retry !"
			return False
		return True
	
	def __str__(self):
		return "x(%s) y(%s) z(%s)" % (self.x, self.y, self.z)

calibration = Calibration()

if not calibration.run():
	print "Calibration failed!"
	exit()

print "Calibration done"
print calibration
