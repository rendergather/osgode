# -*- coding: iso-8859-1 -*-
# file RigidBody.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2012 - 2014 by Rocco Martino                            #
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
from . import ODEObject

import bpy

from mathutils import Vector, Quaternion, Euler
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class RigidBody(ODEObject.ODEObject):
    """osgODE::RigidBody"""





############################################################################
    UpdateActorBound = False
    ActorBoundRadius = None
    Position = Vector()
    Rotation = Quaternion()
    GravityMode = True
    LinearDamping = 0.0
    AngularDamping = 0.0
    LinearDampingThreshold = 0.0
    AngularDampingThreshold = 0.0
    MaxAngularSpeed = None
    Mass = 1.0
    Kinematic = False
    GyroscopicMode = True
    AutoDisableFlag = False
    AutoDisableLinearThreshold = 0.05
    AutoDisableAngularThreshold = 0.05
    AutoDisableTime = 0.1
    AutoDisableSteps = 6
    AutoDisableAverageSamplesCount = 1
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(RigidBody, self).__init__(data, obj)

        self.UpdateActorBound = False
        self.ActorBoundRadius = None
        self.Position = Vector()
        self.Quaternion = Quaternion()
        self.GravityMode = True
        self.LinearDamping = 0.0
        self.AngularDamping = 0.0
        self.LinearDampingThreshold = 0.0
        self.AngularDampingThreshold = 0.0
        self.MaxAngularSpeed = None
        self.Mass = 1.0
        self.Kinematic = False
        self.GyroscopicMode = True
        self.AutoDisableFlag = False
        self.AutoDisableLinearThreshold = 0.01
        self.AutoDisableAngularThreshold = 0.01
        self.AutoDisableTime = 1.0
        self.AutoDisableSteps = 60
        self.AutoDisableAverageSamplesCount = 5
############################################################################




############################################################################
    def buildGraph(self):
        if not super(RigidBody, self).buildGraph():
            return False

        self.UpdateActorBound = self.Object.game.use_actor

        self.ActorBoundRadius = self.Object.game.radius

        self.Position, self.Quaternion, scale = self.Object.matrix_local.decompose()

        self.Mass = self.Object.game.mass


        self.GravityMode = not (self.Object.game.use_ghost or self.Object.game.physics_type == 'NO_COLLISION')


        self.LinearDamping = self.Object.game.damping / bpy.data.scenes[0].render.fps
        self.AngularDamping = self.Object.game.rotation_damping / bpy.data.scenes[0].render.fps
        self.LinearDampingThreshold = self.Object.game.velocity_min
        self.AngularDampingThreshold = self.Object.game.velocity_min

        if self.Object.game.velocity_max > 0.0:
            self.MaxAngularSpeed = self.Object.game.velocity_max


        if self.Object.game.physics_type == 'STATIC':
            self.Kinematic = True


        self.AutoDisableFlag = not self.Object.game.use_sleep
        self.AutoDisableLinearThreshold = self.Data.Context.scene.game_settings.deactivation_linear_threshold
        self.AutoDisableAngularThreshold = self.Data.Context.scene.game_settings.deactivation_angular_threshold
        self.AutoDisableTime = self.Data.Context.scene.game_settings.deactivation_time
        self.AutoDisableSteps = self.Data.Context.scene.game_settings.deactivation_time * self.Data.Context.scene.game_settings.fps
        self.AutoDisableAverageSamplesCount = self.AutoDisableSteps / 10


        from . import ODETransform
        self.ODETransform = ODETransform.ODETransform(self.Data, self.Object)


        self.Data.RigidBodyCache.set(self.Object, self.UniqueID)
        self.Data.RigidBodyMap[self.Object] = self



        for prop in self.Object.game.properties:

            prop_type = ""
            prop_name = prop.name
            prop_value = prop.value

            if prop.type == "STRING":
                prop_type = "String"

            elif prop.type == "FLOAT":
                prop_type = "Float"

            elif prop.type == "INT":
                prop_type = "Int"

            elif prop.type == "BOOL":
                prop_type = "Bool"

                if prop.value:
                    prop_value = "TRUE"
                else:
                    prop_value = "FALSE"


            self.addUserValue( prop_name, prop_value, prop_type )



        try:
            if self.Object["oo_camera_path"]:
                self.addUserValue( "oo_camera_path", 1 )
        except:
            pass

        try:
            v = self.Object["oo_sliding_door"]
            self.addUserValue( "oo_sliding_door", v )
        except:
            pass



        if self.Object.animation_data and self.Object.animation_data.action:

            from . import MotionPath

            mp = MotionPath.MotionPath( self.Data, self.Object )

            mp.buildGraph()

            self.addUpdateCallback( mp )




        return self.ODETransform.buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::RigidBody") ;

        if not self.writePrivateData(writer):
            return False

        writer.moveOut("osgODE::RigidBody")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(RigidBody, self).writePrivateData(writer) :
            return False



        if self.UpdateActorBound :
            writer.writeLine("ActorBound %f %f %f %f" %(self.Position.x, self.Position.y, self.Position.z, self.ActorBoundRadius))



        if self.ODETransform:
            writer.moveIn("MatrixTransform TRUE")

            if not self.ODETransform.writeToStream(writer):
                return False

            writer.moveOut("MatrixTransform TRUE")



        writer.writeLine("Position %f %f %f" %(self.Position.x, self.Position.y, self.Position.z))
        writer.writeLine("Quaternion %f %f %f %f" %(self.Quaternion[1], self.Quaternion[2], self.Quaternion[3], self.Quaternion[0]))

        if not self.GravityMode:
            writer.writeLine("GravityMode FALSE")



        if self.AutoDisableFlag:
            writer.writeLine("AutoDisableFlag TRUE")
        else:
            writer.writeLine("AutoDisableFlag FALSE")

        writer.writeLine("AutoDisableLinearThreshold %f" %self.AutoDisableLinearThreshold)
        writer.writeLine("AutoDisableAngularThreshold %f" %self.AutoDisableAngularThreshold)
        writer.writeLine("AutoDisableSteps %u" %self.AutoDisableSteps)
        writer.writeLine("AutoDisableTime %f" %self.AutoDisableTime)
        writer.writeLine("AutoDisableAverageSamplesCount %i" %self.AutoDisableAverageSamplesCount)



        writer.writeLine("LinearDamping %f" %self.LinearDamping)
        writer.writeLine("AngularDamping %f" %self.AngularDamping)
        writer.writeLine("LinearDampingThreshold %f" %self.LinearDampingThreshold)
        writer.writeLine("AngularDampingThreshold %f" %self.AngularDampingThreshold)

        if self.MaxAngularSpeed != None:
            writer.writeLine("MaxAngularSpeed %f" %self.MaxAngularSpeed)



        writer.writeLine("Mass %f" %self.Mass)



        if self.Kinematic:
            writer.writeLine("Kinematic TRUE")



        if not self.GyroscopicMode:
            writer.writeLine("GyroscopicMode FALSE")


        return True
############################################################################




# ........................................................................ #
############################################################################
