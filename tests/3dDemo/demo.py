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
watch.setSmooth(0.40)

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
		self.speed = soya.Vector(self, 0.0, 0.0, 0.0)
		self.rotation_y_speed = 0.
		self.rotation_x_speed = 0.
		self.rotation_z_speed = 0.
		self.solid = 0
		self.lastmotion = ez430.Motion()

	# Gestion des events
	def begin_round(self):
		soya.Body.begin_round(self)
		motion = motionThread.motion
		if motion != False:
			#self.turn_x(motion.x * 2.20)
			if abs(motion.y) > 2:
				self.turn_y(motion.y)
			#self.turn_z(motion.z)

	def advance_time(self, proportion):
		soya.Body.advance_time(self, proportion)

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
