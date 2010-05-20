#!/usr/bin/env python

# Soya & Python
import sys, os, os.path, soya, soya.sdlconst, soya.cube
import soya.widget as widget
import math
import random

# For the watch works
import ez430
import time
from threading import Thread

ap = ez430.AccessPoint()

# ugly detection, yes...
try:
	ports = ez430.probePorts()
	if len(ports):
		ap.open(ez430.probePorts()[0])
except (RuntimeError):
	print "Unable to connect to watch access point..."

if ap.isOpen():
	if ap.getRadioState() == ez430.RadioState.STOPPED:
		print "Starting access point radio..."
		ap.startRadio()

watch = ez430.Watch(ap.getService())
watch.setSmooth(0.80)
watch.setTimeout(200)

class MotionThread(Thread):
	def __init__ (self):
		Thread.__init__(self)
		self.motion = False
		self.running = True

	def run(self):
		if not ap.isOpen():
			print "Access point not open... stop thread"
			return
		while self.running:
			time.sleep(0.01)
			try:
				self.motion = False
				motion = watch.getMotion()
				self.motion = motion
			except (RuntimeError):
				print "Watch not connected ?"

	def stop(self):
		self.running = False

motionThread = MotionThread()
motionThread.start()

# Initialisation
soya.init(width = 1024, height = 768, sound = 0)
soya.path.append(os.path.join(os.path.dirname(sys.argv[0]), "data"))

# Creates a scene.
scene = soya.World()

class AbsDegree:
	def __init__(self):
		self.absdegree = 0

	def update(self, new):
		r = new - self.absdegree
		self.absdegree = new
		return r

#Classe Demo
class Demo(soya.Body):

	# Initialisation Object (constructeur)
	def __init__(self, parent):
		soya.Body.__init__(self, parent)
		self.models = []
		
		# cube
		material = soya.Material()
		material.shininess = 0.5
		material.diffuse   = (0.0, 0.2, 0.7, 1.0)
		material.specular  = (0.2, 0.7, 1.0, 1.0)
		material.separate_specular = 1
		cube = soya.cube.Cube(None, material)
		cube_model = cube.to_model()

		# models
		self.models.append(cube_model)
		self.models.append(soya.Model.get("sword"))
		self.modelidx = 0
		self.set_model(self.models[self.modelidx])

		self.adx = AbsDegree()
		self.ady = AbsDegree()

		self.btnlatency = 0

	def nextmodel(self):
		self.modelidx += 1
		if self.modelidx >= len(self.models):
			self.modelidx = 0
		self.set_model(self.models[self.modelidx])

	# Gestion des events
	def begin_round(self):
		soya.Body.begin_round(self)

		#for event in soya.MAIN_LOOP.events:
		#	if event[0] == soya.sdlconst.KEYUP:
		#		if event[1] == soya.sdlconst.K_e:
		#			self.set_model(soya.Model.get("sword"))


		motion = motionThread.motion
		if motion != False:
		#motion = ez430.Motion()
		#if watch.tryGetMotion(motion):
			if self.btnlatency == 0 and motion.button == ez430.Button.UP:
				self.nextmodel()
				self.btnlatency = 10
			if self.btnlatency > 0:
				self.btnlatency -= 1

			dx = (motion.x * 90 / 60) + 90
			dy = -(motion.y * 90 / 60)

			self.set_identity()
			self.x = 0.
			self.y = -1.
			self.z = -4.
			self.turn_x(dx)
			self.turn_y(dy)

	def advance_time(self, proportion):
		soya.Body.advance_time(self, proportion)

# Construction Vrai Sword
demo = Demo(scene)

# Parametrage Sword
demo.x = 0.
demo.y = -1.
demo.z = -4.
demo.turn_y(90)
demo.turn_x(90)

# Gestion de la lumiere
light = soya.Light(scene)
light.set_xyz( 5., 5., 0.)
light.directional = 2
light.diffuse = (1.0, 0.8, 0.4, 1.0)
light.rotate_x(-90.0)

# Atmosphere 
scene.atmosphere = soya.SkyAtmosphere()
scene.atmosphere.ambient = (0.3, 0.3, 0.4, 1.0)
scene.atmosphere.bg_color = (0.2, 0.5, 0.7, 1.0)

scene.atmosphere.skyplane  = 1
scene.atmosphere.sky_color = (1.5, 1.0, 0.8, 1.0)
scene.atmosphere.fog = 1
scene.atmosphere.fog_type  = 0
scene.atmosphere.fog_start = 40.0
scene.atmosphere.fog_end   = 50.0
scene.atmosphere.fog_color = (0.3, 0.3, 0.4, 1.0) 


# Gestion de la Camera
camera = soya.Camera(scene)
camera.x = 0.
camera.y = 0.
camera.z = 0.

#camera.look_at(demo)

# Application de la camera a la l'application
soya.set_root_widget(camera)

# Main Loop
try:
	soya.MainLoop(scene).main_loop()
except (KeyboardInterrupt, SystemExit):
	print "Exiting..."
	motionThread.stop()
