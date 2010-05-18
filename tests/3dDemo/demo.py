#!/usr/bin/env python


#
# TODO
# - Refasse la classe Weapon
# - Ajout possibiliter d'action spesifique avec bouton specifique
# - Affichage de box qui log les informations clavier
# - Bouton pour changer d'item

# - Backtravelling Camera (reset position)
# - Chargement lightsaber
# - Reparation du fog
# - Mise en place de support // Damier
# - Virer le bug d'execution
#


import sys, os, os.path, soya, soya.sdlconst
import soya.widget as widget

# Initialisation
soya.init()
soya.path.append(os.path.join(os.path.dirname(sys.argv[0]), "data"))

# Creates a scene.
scene = soya.World()

# Classe personaliser pour la gestion de l'epee
class ArticulateSword(soya.Body):
	def __init__(self, parent):
#		soya.Body.__init__(self, parent, soya.Model.get("sword"))
		soya.Body.__init__(self, parent, soya.Model.get("katana"))
		self.speed = soya.Vector(self, 0.0, 0.0, 0.0)
		self.rotation_y_speed = 0.0
		self.rotation_x_speed = 0.0
		self.rotation_z_speed = 0.0
		self.solid = 0
        
	def begin_round(self):
		soya.Body.begin_round(self)
		for event in soya.MAIN_LOOP.events:
			if event[0] == soya.sdlconst.KEYDOWN:
				if   event[1] == soya.sdlconst.K_UP:     self.rotation_x_speed = -10.0
				elif event[1] == soya.sdlconst.K_DOWN:   self.rotation_x_speed = 10.0
				elif event[1] == soya.sdlconst.K_LEFT:   self.rotation_y_speed = -10.0
				elif event[1] == soya.sdlconst.K_RIGHT:  self.rotation_y_speed = 10.0
				elif event[1] == soya.sdlconst.K_LSHIFT: self.reset_pos()
			elif event[0] == soya.sdlconst.KEYUP:
				if   event[1] == soya.sdlconst.K_UP:     self.rotation_x_speed = 0.0
				elif event[1] == soya.sdlconst.K_DOWN:   self.rotation_x_speed = 0.0
				elif event[1] == soya.sdlconst.K_LEFT:   self.rotation_y_speed = 0.0
				elif event[1] == soya.sdlconst.K_RIGHT:  self.rotation_y_speed = 0.0
			elif event[0] == soya.sdlconst.QUIT:
				soya.MAIN_LOOP.stop()

		self.rotate_y(self.rotation_y_speed)
		self.rotate_x(self.rotation_x_speed)

	def advance_time(self, proportion):
		soya.Body.advance_time(self, proportion)
		self.add_mul_vector(proportion, self.speed)

	def reset_pos(self):
		print "Reset des position"

	def aff_data(self):
		print "Affichage des donnees a chaque"

# Construction Vrai Sword
sword = ArticulateSword(scene)

# Parametrage Sword
sword.z = -30.
sword.y = -30.
sword.x = -30.
sword.rotate_y(90.0)
sword.rotate_z(-90.0)

# Gestion de la lumiere
light = soya.Light(scene)
light.set_xyz( .5, 0., 2.)
light.directional = 1
light.diffuse = (1.0, 0.8, 0.4, 1.0)
light.rotate_x(-45.0)

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
camera.x = -5.
camera.y = -5.
camera.z = -5.

#traveling = soya.ThirdPersonTraveling(sword)
#traveling.distance = 15.0

#camera.add_traveling(traveling)
#camera.speed = 0.3
#camera.x = -5.
#camera.y = -5.
#camera.z = -5.


camera.look_at(sword)

# Application de la camera a la l'application
soya.set_root_widget(camera)

# Main Loop
soya.MainLoop(scene).main_loop()

