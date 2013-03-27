# -*- coding: iso-8859-1 -*-
# file TargetJoint.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2013 by Rocco Martino                                   #
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

from mathutils import Vector, Matrix, Euler
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class TargetJoint(ODEObject.ODEObject):
    """TargetJoint in the scene"""





############################################################################
    TransformUniqueID = None
    MotionPath = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(TargetJoint, self).__init__(data, obj)

        self.TransformUniqueID = None
        self.MotionPath = None
############################################################################




############################################################################
    def buildGraph(self):
        super(TargetJoint, self).buildGraph()



        self.TransformUniqueID = self.Data.UniqueID.generate()


        from . import MotionPath

        self.MotionPath = MotionPath.MotionPath(self.Data, self.Object)

        self.MotionPath.buildGraph()


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        if not self.Object:
            return False

        writer.moveIn("osgODE::TargetJoint")

        if not self.writePrivateData(writer):
            return False

        writer.moveOut("osgODE::TargetJoint")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(TargetJoint, self).writePrivateData(writer) :
            return False



        if self.MotionPath:
            writer.moveIn("UpdateCallback TRUE")
            self.MotionPath.writeToStream(writer)
            writer.moveOut("UpdateCallback TRUE")


        writer.moveIn("MatrixTransform TRUE")
        writer.moveIn("osg::MatrixTransform")
        writer.writeLine( "UniqueID %s" % str(self.TransformUniqueID) )
        writer.moveOut("osg::MatrixTransform")
        writer.moveOut("MatrixTransform TRUE")



        if self.Object:
            writer.moveIn("Body1 TRUE")
            writer.moveIn("osgODE::RigidBody")
            writer.writeLine("UniqueID %d" %self.Data.RigidBodyCache.get(self.Object)) ;
            writer.moveOut("osgODE::RigidBody")
            writer.moveOut("Body1 TRUE")



        writer.writeLine("dParamCFM 0.0")
        writer.writeLine("dParamERP 1.0")



        return True
############################################################################




# ........................................................................ #
############################################################################
