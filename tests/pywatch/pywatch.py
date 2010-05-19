#!/usr/bin/env python

import ez430

ap = ez430.AccessPoint()

ap.open("/dev/ttyACM0")

print "%x" % (ap.getProductId())

print "Current radio state: " + str(ap.getRadioState())

if ap.getRadioState() == ez430.RadioState.STOPPED:
	print "Starting radio..."
	ap.startRadio()

watch = ez430.Watch(ap.getService())
watch.setSmooth(0.80)

try:
	while 1:
		try:
			while 1:
				motion = watch.getMotion()
				print motion
		except RuntimeError:
			print "Link timeout... retry !"
except (KeyboardInterrupt, SystemExit):
	pass
