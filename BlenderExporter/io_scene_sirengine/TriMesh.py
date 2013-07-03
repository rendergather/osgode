# -*- coding: iso-8859-1 -*-
# file TriMesh.py
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
from . import Collidable
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class TriMesh(Collidable.Collidable):
    """osgODE::TriMesh"""





############################################################################
    TriMeshData = None
    ReadOK = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(TriMesh, self).__init__(data, obj)
############################################################################




############################################################################
    def buildGraph(self):
        if not super(TriMesh, self).buildGraph():
            return False


        from . import TriMeshData
        self.TriMeshData = TriMeshData.TriMeshData()
        self.ReadOK = self.TriMeshData.read(self.Object)



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::TriMesh")

        if not self.writePrivateData(writer):
            return False



        if not self.ReadOK:
            writer.moveOut("osgODE::TriMesh")

            return True


        VERTICES = "VertexArray %d"  % len(self.TriMeshData.VertexArray)
        INDICES = "IndexArray %d"  % len(self.TriMeshData.IndexArray)




        writer.moveIn(VERTICES)

        for v in self.TriMeshData.VertexArray:
            writer.writeLine("%f %f %f" % (v.x, v.y, v.z))

        writer.moveOut(VERTICES)




        writer.moveIn(INDICES)


        for i in range( 0, len(self.TriMeshData.IndexArray), 3 ):
            i1 = self.TriMeshData.IndexArray[i]
            i2 = self.TriMeshData.IndexArray[i+1]
            i3 = self.TriMeshData.IndexArray[i+2]

            writer.writeLine( "%d %d %d" % (i1, i2, i3) )


        writer.moveOut(INDICES)




        writer.moveOut("osgODE::TriMesh")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(TriMesh, self).writePrivateData(writer) :
            return False


        return True
############################################################################




# ........................................................................ #
############################################################################
