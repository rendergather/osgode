# -*- coding: iso-8859-1 -*-
# file ConvexPlanarOccluder.py
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
from . import Writable
from mathutils import Vector
import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class ConvexPlanarOccluder(Writable.Writable):
    """ConvexPlanarOccluder in the scene"""





############################################################################
    Object = None
    VertexArray = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(ConvexPlanarOccluder, self).__init__(data)

        self.Object = obj
        self.VertexArray = []
############################################################################




############################################################################
    def buildGraph(self):
        super(ConvexPlanarOccluder, self).buildGraph()


        mesh = self.Object.data

        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            faces = mesh.polygons
        else:
            faces = mesh.faces

        vertices = mesh.vertices

        if len(faces) != 1:
            print("This exporter only supports single face occluders")
            return False

        f = faces[0]

        num_vert = len(f.vertices)

        if not (num_vert == 3 or num_vert == 4):
            print("This exporter only supports triangle and quad occluders")
            return False


        for i in range(0, num_vert, 1):
            self.VertexArray.append( self.Object.matrix_world * Vector(vertices[f.vertices[i]].co) )


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::ConvexPlanarOccluder") ;

        if not super(ConvexPlanarOccluder, self).writeToStream(writer) :
            return False


        num_vert = len(self.VertexArray)

        if num_vert > 0:
            writer.moveIn("Occluder %d" %num_vert)

            for v in self.VertexArray:
                writer.writeLine("%f %f %f" %(v.x, v.y, v.z))

            writer.moveOut("Occluder %d" %num_vert)



        writer.moveOut("osg::ConvexPlanarOccluder")


        return True
############################################################################




# ........................................................................ #
############################################################################
