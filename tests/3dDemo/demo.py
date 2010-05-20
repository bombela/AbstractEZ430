#!/usr/bin/env python

# Soya & Python
import sys, os, os.path, soya, soya.sdlconst
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

#Classe Weapon
class Weapon(soya.Body):

	# Initialisation Object (constructeur)
	def __init__(self, parent, w_name):
		soya.Body.__init__(self, parent)
		self.set_model(soya.Model.get(w_name))
		self.speed = soya.Vector(parent, 0.0, 0.0, 0.0)
#		self.p_speed = soya.Vector(parent, 0.,0.,0.) # previous speed

		self.r_speed = soya.Vector(parent, 0.,0.,0.) # rotation speed
		self.p_accel = soya.Vector(parent, 0.,0.,0.) # pourcentage accel speed
		self.p_speed = soya.Vector(parent, 0.,0.,0.) # pourcentage accel speed
#		self.p_r_speed = soya.Vector(parent, 0.,0.,0.) # previous rotation speed
		self.solid = 0
		self.lastmotion = ez430.Motion()
		self.a = 0 # angle
		self.b = 0

		self.adx = AbsDegree()
		self.ady = AbsDegree()

	# Gestion des events
	def begin_round(self):
		soya.Body.begin_round(self)

		self.speed.x = 0.
		self.speed.y = 0.
		self.speed.z = 0.

		self.r_speed.x = 0.
		self.r_speed.y = 0.
		self.r_speed.z = 0.

		self.p_accel.x = 0.
		self.p_accel.x = 0.
		self.p_accel.x = 0.

		self.a = 0 # angle

		for event in soya.MAIN_LOOP.events:
			if event[0] == soya.sdlconst.KEYUP:
				if event[1] == soya.sdlconst.K_a:
					self.rotate(90,
							soya.Point(scene, 0.0, 0.0, 0.0),
							soya.Point(scene, 0.0, -1.0, -4.0))
				elif event[1] == soya.sdlconst.K_z:
					self.rotate(90,
							soya.Point(scene, 0.0, 10.0, -4.0),
							soya.Point(scene, 0.0, -10.0, -4.0))
				elif event[1] == soya.sdlconst.K_e:
					self.set_identity()


		motion = motionThread.motion
		if motion != False:
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
#		self.add_vector(self.speed)
#		self.rotate_axis(self.a, self.speed)
		self.rotate_x(self.r_speed.x)
		self.rotate_y(self.r_speed.y)
		self.rotate_z(self.r_speed.z)
#		if self.motion != False:
#			self.add_mul_vector(proportion, self.speed)

	# Affichage des donnees recues
	def aff_data(self):
		print "Received Data -> button: x: y: z:"

	# Fonction Button 1 Pressed
	def funcion_1(self):
		print "Fonction Button 1 Pressed"

	# Fonction Button 2 Pressed
	def funcion_2(self):
		print "Fonction Button 2 Pressed"

	# Fonction Button 3 Pressed
	def funcion_3(self):
		print "Fonction Button 3 Pressed"

# Construction Vrai Sword
weapon = Weapon(scene, "sword")

# Parametrage Sword
weapon.x = 0.
weapon.y = -1.
weapon.z = -4.
weapon.turn_y(90)
weapon.turn_x(90)

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

#camera.look_at(weapon)

# Application de la camera a la l'application
soya.set_root_widget(camera)

# Main Loop
try:
	soya.MainLoop(scene).main_loop()
except (KeyboardInterrupt, SystemExit):
	print "Exiting..."
	motionThread.stop()
