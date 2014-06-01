# -*- coding: iso-8859-1 -*-
# file Space.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2012 by Rocco Martino                                   #
#    martinorocco@gmail.com                                                #
#                                                                          #
#    This program is free software; you can redistribute it and/or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################

############################################################################
from . import Writable, Game, NearCallback, Joint

import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Space(Writable.Writable):
    """osgODE::Space"""





############################################################################
    NearCallback = None
    ODEObjects = []
    Gravity = -9.80665
    ID = 0
    ERP = 0.2
    CFM = 1.0e-5
    RigidBodies = []
############################################################################









############################################################################
    def __init__(self, data):
        super(Space, self).__init__(data)

        self.NearCallback = None
        self.ODEObjects = []
        self.Gravity = -9.80665
        self.ID = 0
        self.ERP = 0.2
        self.CFM = 1.0e-5
        self.RigidBodies = []
############################################################################




############################################################################
    def buildGraph(self):
        super(Space, self).buildGraph()

        self.ID = self.Data.ODEID.generate()

        self.Gravity = -self.Data.Context.scene.game_settings.physics_gravity

        self.ERP = 0.2
        self.CFM = 1.0e-5

        self.NearCallback = NearCallback.NearCallback(self.Data)
        self.addChild(self.NearCallback)




        rigid_bodies = []
        empties = []


        for obj in self.Data.Scene.objects:
            if obj.is_visible( self.Data.Scene ) :
                if obj.type != "EMPTY" :
                    if not self.addObject(obj):
                        return False
                    else:
                        rigid_bodies.append(obj)
                else:
                    empties.append( obj )


        for obj in empties:
            if not self.addObject(obj):
                return False
            else:
                rigid_bodies.append(obj)



        for rigid_body in rigid_bodies:
            constraints = rigid_body.constraints

            for constraint in constraints:
                joint = Joint.Joint(self.Data, constraint, rigid_body)
                self.addChild(joint)
                self.ODEObjects.append(joint)



        if not self.traverseBuild():
            return False


        if self.Data.ExportGame:

            for rigid_body in self.RigidBodies:

                if ( len(rigid_body.Object.game.sensors) + len(rigid_body.Object.game.properties) ) > 0:

                    game = Game.Game( rigid_body )
                    game.buildGraph()

                    rigid_body.addUpdateCallback( game )


        return True
############################################################################




############################################################################
    def addObject(self, obj):

        if obj.type == 'MESH':

            num_materials = len(obj.material_slots)

            if num_materials > 1:
                self.Data.Operator.report({'ERROR'}, "[%s] This exporter supports only 1 material per mesh" %obj.name)
                return False



            is_graphics = False

            if num_materials == 0:
                is_graphics = True
            elif not obj.material_slots[0].material:
                is_graphics = True
            elif not obj.material_slots[0].material.game_settings.physics:
                is_graphics = True


            if not is_graphics:
                from . import ODEObject
                physics = self.selectODEObject(obj)

                if physics:
                    self.addChild(physics)
                    self.ODEObjects.append(physics)
                    self.RigidBodies.append(physics)


                if self.Data.Context.selected_objects and self.Data.Context.selected_objects[0] == obj:
                    self.Data.Selected = physics




        elif obj.type == 'EMPTY':

            from . import ODEObject
            physics = self.selectODEObject(obj)

            if physics:
                self.addChild(physics)
                self.ODEObjects.append(physics)
                self.RigidBodies.append(physics)


            if self.Data.Context.selected_objects and self.Data.Context.selected_objects[0] == obj:
                self.Data.Selected = physics



        return True
############################################################################




############################################################################
    def selectODEObject(self, obj):

        if obj.parent != None:
            return None


        elif not obj.game.use_collision_bounds:

            from . import RigidBody
            return RigidBody.RigidBody(self.Data, obj)



        elif obj.game.physics_type == 'NO_COLLISION' :

            from . import RigidBody
            return RigidBody.RigidBody(self.Data, obj)





        elif obj.game.physics_type in ['STATIC', 'DYNAMIC', 'RIGID_BODY']:
            if obj.game.collision_bounds_type == 'TRIANGLE_MESH':
                from . import TriMesh
                return TriMesh.TriMesh(self.Data, obj)

            elif obj.game.collision_bounds_type == 'BOX':
                from . import Box
                return Box.Box(self.Data, obj)

            elif obj.game.collision_bounds_type == 'SPHERE':
                from . import Sphere
                return Sphere.Sphere(self.Data, obj)

            elif obj.game.collision_bounds_type == 'CYLINDER':
                from . import Cylinder
                return Cylinder.Cylinder(self.Data, obj)

            elif obj.game.collision_bounds_type == 'CAPSULE':
                from . import Capsule
                return Capsule.Capsule(self.Data, obj)

            else:
                print("This exporter does not support %s collision bounds" %obj.game.collision_bounds_type)


            pass



        elif obj.game.physics_type in ['OCCLUDE']:
            # Esprtato in OccluderGroup
            return None



        else:
            print("This exporter does not support %s physics type" %obj.game.physics_type)
            return None
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osgODE::Space") ;

        if not super(Space, self).writeToStream(writer) :
            return False


        writer.writeLine("ID %d" %self.ID)

        writer.writeLine("Gravity 0 0 %f" %self.Gravity)

        writer.writeLine("ERP %f" %self.ERP)
        writer.writeLine("CFM %f" %self.CFM)


        writer.writeLine("WorldStepFunction %s" %self.Data.WorldStep)


        if len(self.ODEObjects):
            writer.writeLine("ODEObjects %d" %len(self.ODEObjects))

        for obj in self.ODEObjects:
            if not obj.writeToStream(writer):
                return False


        if self.Data.ExportGame:
            writer.moveIn("Events TRUE")
            writer.moveIn("osgODE::Events")
            writer.writeLine("UniqueID %d" % self.Data.UniqueID.generate() )
            writer.moveOut("osgODE::Events")
            writer.moveOut("Events TRUE")


        writer.moveIn("NearCallback TRUE")

        if not self.NearCallback.writeToStream(writer):
            return False

        writer.moveOut("NearCallback TRUE")



        writer.moveOut("osgODE::Space")

        return True
############################################################################




# ........................................................................ #
############################################################################
