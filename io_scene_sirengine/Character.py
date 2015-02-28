# -*- coding: iso-8859-1 -*-
# file Character.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2014 by Rocco Martino                                   #
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Character(ODEObject.ODEObject):
    """osgODE::Character"""





############################################################################
    Physics = None
    ClassName = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Character, self).__init__(data, obj)

        self.Physics = None
        self.ClassName = None

        self.IsCharacter = True
############################################################################




############################################################################
    def buildGraph(self):
        if not super(Character, self).buildGraph():
            return False




        self.Physics = self.selectODEObject( self.Object )

        if self.Physics:
            self.Physics.buildGraph()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::Container") ;

        if not self.writePrivateData(writer):
            return False


        if self.Physics:

            writer.writeLine("ODEObjects 2")

            self.Physics.writeToStream( writer )




            writer.moveIn("osgODE::FixedJoint")

            writer.writeLine("UniqueID %d" % self.Data.UniqueID.generate())
            writer.writeLine("ID %d" % self.Data.ODEID.generate())



            writer.moveIn("Body1 TRUE")

            writer.moveIn(self.ClassName)
            writer.writeLine("UniqueID %d" % self.Physics.UniqueID)
            writer.moveOut(self.ClassName)

            writer.moveOut("Body1 TRUE")


            writer.writeLine("PositionMode FALSE")

            writer.moveOut("osgODE::FixedJoint")





        writer.moveOut("osgODE::Container")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Character, self).writePrivateData(writer) :
            return False


        return True
############################################################################




############################################################################
    def selectODEObject(self, obj):

        if obj.parent != None:
            return None


        elif not obj.game.use_collision_bounds:

            self.ClassName = "osgODE::RigidBody"

            from . import RigidBody
            return RigidBody.RigidBody(self.Data, obj)



        elif obj.game.physics_type == 'NO_COLLISION' :

            self.ClassName = "osgODE::RigidBody"

            from . import RigidBody
            return RigidBody.RigidBody(self.Data, obj)





        elif obj.game.physics_type in ['STATIC', 'DYNAMIC', 'RIGID_BODY', 'CHARACTER']:
            if obj.game.collision_bounds_type == 'TRIANGLE_MESH':
                self.ClassName = "osgODE::TriMesh"
                from . import TriMesh
                return TriMesh.TriMesh(self.Data, obj)

            elif obj.game.collision_bounds_type == 'BOX':
                self.ClassName = "osgODE::Box"
                from . import Box
                return Box.Box(self.Data, obj)

            elif obj.game.collision_bounds_type == 'SPHERE':
                self.ClassName = "osgODE::Sphere"
                from . import Sphere
                return Sphere.Sphere(self.Data, obj)

            elif obj.game.collision_bounds_type == 'CYLINDER':
                self.ClassName = "osgODE::Cylinder"
                from . import Cylinder
                return Cylinder.Cylinder(self.Data, obj)

            elif obj.game.collision_bounds_type == 'CAPSULE':
                self.ClassName = "osgODE::Capsule"
                from . import Capsule
                return Capsule.Capsule(self.Data, obj)

            else:
                print("This exporter does not support %s collision bound" %obj.game.collision_bounds_type)


            pass



        elif obj.game.physics_type in ['OCCLUDE']:
            # Esprtato in OccluderGroup
            return None



        else:
            print("This exporter does not support %s physics type" %obj.game.physics_type)
            return None
############################################################################




# ........................................................................ #
############################################################################
