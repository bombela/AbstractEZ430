#!/usr/bin/env python

#
# TODO
# - Chargement lightsaber
#

import sys, os, os.path, soya, soya.sdlconst
import soya.widget as widget
import ez430
import time
from threading import Thread
import math
import random

ap = ez430.AccessPoint()

# ugly detection, yes...
try:
	if os.name == "posix":
		ap.open("/dev/ttyACM0")
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
			time.sleep(0.1)
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
#soya.init(width = 1024, height = 768)
soya.init()
soya.path.append(os.path.join(os.path.dirname(sys.argv[0]), "data"))

# Creates a scene.
scene = soya.World()


#Classe Weapon
class Weapon(soya.Body):

	# Initialisation Object (constructeur)
	def __init__(self, parent, w_name):
		soya.Body.__init__(self, parent, soya.Model.get(w_name))
		self.speed = soya.Vector(parent, 0.0, 0.0, 0.0)
		self.p_speed = soya.Vector(parent, 0.,0.,0.) # previous speed

		self.r_speed = soya.Vector(parent, 0.,0.,0.) # rotation speed
		self.p_r_speed = soya.Vector(parent, 0.,0.,0.) # previous rotation speed
		self.solid = 0
		self.lastmotion = ez430.Motion()
		self.rotation_x_speed = 0
		self.rotation_y_speed = 0
		self.rotation_z_speed = 0
		self.a = 0 # angle
		self.motion = motionThread.motion

	# Calcule de la difference entre ancien et nouveaux
	def difference(self):
		def __init__(self):
			self.diff = False
		def calc(self, old, new):
			

	# Gestion des events
	def begin_round(self):
		soya.Body.begin_round(self)
		self.speed.x = 0.
		self.speed.x = 0.
		self.speed.x = 0.

		for event in soya.MAIN_LOOP.events:
			if event[0] == soya.sdlconst.KEYDOWN:
				# Translation
				if   event[1] == soya.sdlconst.K_UP:     self.speed.z =  0.1
				elif event[1] == soya.sdlconst.K_DOWN:   self.speed.z =  -0.2
				elif event[1] == soya.sdlconst.K_LEFT:   self.speed.x = 0.1
				elif event[1] == soya.sdlconst.K_RIGHT:  self.speed.x = -0.2
				elif event[1] == soya.sdlconst.K_w:   self.speed.y = 0.1
				elif event[1] == soya.sdlconst.K_s:  self.speed.y = -0.2

				# Rotation
				elif event[1] == soya.sdlconst.K_a:   self.r_speed.y = -10.0
				elif event[1] == soya.sdlconst.K_d:  self.r_speed.y =  10.0
				elif event[1] == soya.sdlconst.K_q:   self.r_speed.x = -10.0
				elif event[1] == soya.sdlconst.K_e:  self.r_speed.x =  10.0
				elif event[1] == soya.sdlconst.K_z:   self.r_speed.z = -10.0
				elif event[1] == soya.sdlconst.K_c:  self.r_speed.z =  10.0

#		self.previous = soya.Vector(parent, self.speed.x, self.speed.y, self.speed.z)
#		self.motion = motion = motionThread.motion
#		if motion != False:		
#			h = math.hypot(motion.x, motion.y) # hypotenuse
#			if h != 0:
#				self.a = math.degrees(math.asin(motion.y / h))
#			else:
#				self.a = 90
#			print " x:%d" %motion.x
#			print " y:%d" %motion.y
#			print " z:%d" %motion.z
#			print " h:%d" %h
#			print " a:%d" %self.a


#			self.speed.x = 0
#			self.speed.y = 0		
#			self.speed.z = (motion.z * -1) / 100
#			self.add_vector(self.speed)

#			self.speed.x = 0
#			self.speed.y = motion.x / 100
#			self.speed.z = 0
#			self.add_vector(self.speed)

#			self.rotation_speed = random.uniform(-25., 25.)
#			self.rotate_axis(self.a, self.speed)

	def advance_time(self, proportion):
		soya.Body.advance_time(self, proportion)
		self.add_vector(self.speed)
		self.rotate_x(self.rotation_x_speed)
		self.rotate_y(self.rotation_y_speed)
		self.rotate_z(self.rotation_z_speed)
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
	
	def init_xyz(self, x, y,z):
		self.before_x = x
		self.before_y = y
		self.before_z = z

# Construction Vrai Sword
weapon = Weapon(scene, "katana")

# Parametrage Sword
weapon.x = 0.
weapon.y = 0.
weapon.z = 20.
#sword.rotate_y(90.0)
#sword.rotate_z(-90.0)

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
camera.z = 5.

camera.look_at(weapon)

# Application de la camera a la l'application
soya.set_root_widget(camera)

# Main Loop
try:
	soya.MainLoop(scene).main_loop()
except (KeyboardInterrupt, SystemExit):
	print "Exiting..."
	motionThread.stop()
