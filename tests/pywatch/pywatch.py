#!/usr/bin/env python

import ez430

ap = ez430.AccessPoint()

ap.open("/dev/ttyACM0")

print "%x" % (ap.getProductId())

print ap.getRadioState()

ap.getService()
