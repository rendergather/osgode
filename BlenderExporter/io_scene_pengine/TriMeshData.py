# -*- coding: iso-8859-1 -*-
# file TriMeshData.py
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
from mathutils import Vector
import bpy
############################################################################




############################################################################
# ........................................................................ #
STATIC_CATEGORY_BITS = 1
DYNAMIC_CATEGORY_BITS = 2
STATIC_COLLIDE_BITS = DYNAMIC_CATEGORY_BITS
DYNAMIC_COLLIDE_BITS = DYNAMIC_CATEGORY_BITS | STATIC_CATEGORY_BITS

NORMAL_MIN_LENGTH = 1.0e-3
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class TriMeshData(object):
    """Vertex and index array of a triangle mesh"""





############################################################################
    VertexArray = []
    IndexArray = []
############################################################################









############################################################################
    def __init__(self):
        self.VertexArray = []
        self.IndexArray = []
############################################################################




############################################################################
    def read(self, obj):
        self.VertexArray = self.readVertexArray(obj)
        self.IndexArray = self.readIndexArray(obj)


        return( self.VertexArray and
                self.IndexArray )
############################################################################




############################################################################
    def readVertexArray(self, obj):

        vertex_array = []

        vertices = obj.data.vertices



        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            faces = obj.data.polygons
        else:
            faces = obj.data.faces



        # traslo qui i vertici perché non posso avere una scala nella matrice

        scale_vec = obj.matrix_world.to_scale()

        for v in vertices:
            vert = Vector(v.co)
            vert.x *= scale_vec.x
            vert.y *= scale_vec.y
            vert.z *= scale_vec.z
            vertex_array.append(vert)




        return vertex_array
############################################################################




############################################################################
    def readIndexArray(self, obj):
        index_array = []

        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            faces = obj.data.polygons
        else:
            faces = obj.data.faces


        for f in faces:
            vertices = f.vertices
            num = len(vertices)

            if num == 3:
                v0 = self.VertexArray[vertices[0]]
                v1 = self.VertexArray[vertices[1]]
                v2 = self.VertexArray[vertices[2]]

                vt = v0 - v1
                vb = v0 - v2

                vt.normalize()
                vb.normalize()

                t = [0,0,0]

                t[0] = vt[0] * vb[1] - vt[1] * vb[0]
                t[1] = vt[2] * vb[0] - vt[0] * vb[2]
                t[2] = vt[1] * vb[2] - vt[2] * vb[1]

                l = t[0]*t[0] + t[1]*t[1] + t[2]*t[2]

                if(l >= NORMAL_MIN_LENGTH):
                    index_array.append(vertices[0])
                    index_array.append(vertices[1])
                    index_array.append(vertices[2])
                else:
                    print("%s: %lf < NORMAL_MIN_LENGTH: triangle not exported" %(obj.name, l))
            elif num == 4:
                v0 = self.VertexArray[vertices[0]]
                v1 = self.VertexArray[vertices[1]]
                v2 = self.VertexArray[vertices[2]]

                vt = v0 - v1
                vb = v0 - v2

                vt.normalize()
                vb.normalize()

                t = [0.0,0.0,0.0]

                t[0] = vt[0] * vb[1] - vt[1] * vb[0]
                t[1] = vt[2] * vb[0] - vt[0] * vb[2]
                t[2] = vt[1] * vb[2] - vt[2] * vb[1]

                l = t[0]*t[0] + t[1]*t[1] + t[2]*t[2]

                if(l >= NORMAL_MIN_LENGTH):
                    index_array.append(vertices[0])
                    index_array.append(vertices[1])
                    index_array.append(vertices[2])
                else:
                    print("%s: %lf < NORMAL_MIN_LENGTH: triangle not exported" %(obj.name, l))





                v0 = self.VertexArray[vertices[0]]
                v1 = self.VertexArray[vertices[2]]
                v2 = self.VertexArray[vertices[3]]

                vt = v0 - v1
                vb = v0 - v2

                vt.normalize()
                vb.normalize()

                t = [0,0,0]

                t[0] = vt[0] * vb[1] - vt[1] * vb[0]
                t[1] = vt[2] * vb[0] - vt[0] * vb[2]
                t[2] = vt[1] * vb[2] - vt[2] * vb[1]

                l = t[0]*t[0] + t[1]*t[1] + t[2]*t[2]



                if(l >= NORMAL_MIN_LENGTH):
                    index_array.append(vertices[0])
                    index_array.append(vertices[2])
                    index_array.append(vertices[3])
                else:
                    print("%s: %lf < NORMAL_MIN_LENGTH: triangle not exported" %(obj.name, l))



        return index_array
############################################################################




# ........................................................................ #
############################################################################
