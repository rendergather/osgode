# -*- coding: iso-8859-1 -*-
# file Collidable.py
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
from . import RigidBody

from mathutils import Vector
############################################################################




############################################################################
# ........................................................................ #
#STATIC_BIT = 1
#DYNAMIC_BIT = 2
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Collidable(RigidBody.RigidBody):
    """osgODE::Collidable"""





############################################################################
    Size = Vector()
    CategoryBits = 0
    CollideBits = 0
    CollisionParameters = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Collidable, self).__init__(data, obj)

        self.Size.x = 1
        self.Size.y = 1
        self.Size.z = 1
        self.CategoryBits = 0
        self.CollideBits = 0
        self.CollisionParameters = None
############################################################################




############################################################################
    def buildGraph(self):
        if not super(Collidable, self).buildGraph():
            return False

        self.Size = Vector(self.Object.dimensions)

        #if self.Object.game.physics_type == 'STATIC':
            #self.CategoryBits = STATIC_BIT
            #self.CollideBits = DYNAMIC_BIT
        #else:
            #self.CategoryBits = DYNAMIC_BIT
            #self.CollideBits = DYNAMIC_BIT | STATIC_BIT




        self.CollideBits = 0
        self.CategoryBits = 0





        temp = 1

        for i in self.Object.game.collision_group:
            if i:
                self.CategoryBits = self.CategoryBits | temp
            temp = temp << 1




        temp = 1

        for i in self.Object.game.collision_mask:
            if i:
                self.CollideBits = self.CollideBits | temp
            temp = temp << 1




        from . import CollisionParameters
        self.CollisionParameters = CollisionParameters.CollisionParameters(self.Data)

        num_materials = len(self.Object.material_slots)

        if num_materials > 0:
            self.CollisionParameters.Mu = self.Object.material_slots[0].material.physics.friction
            self.CollisionParameters.Bounce = self.Object.material_slots[0].material.physics.elasticity


        return self.CollisionParameters.buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::Collidable") ;

        if not self.writePrivateData(writer):
            return False


        writer.moveOut("osgODE::Collidable")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Collidable, self).writePrivateData(writer) :
            return False



        writer.writeLine("CategoryBits 0x%x" %self.CategoryBits)
        writer.writeLine("CollideBits 0x%x" %self.CollideBits)
        writer.writeLine("GeometrySize %f %f %f" %(self.Size.x, self.Size.y, self.Size.z))


        writer.moveIn("CollisionParameters TRUE")

        if not self.CollisionParameters.writeToStream(writer):
            return False

        writer.moveOut("CollisionParameters TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################
