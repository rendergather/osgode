# -*- coding: iso-8859-1 -*-
# file MeshData.py
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
from mathutils import Color, Vector
import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class MeshData(object):
    """Vertex, normal and color array of a mesh"""





############################################################################
    VertexArray = []
    NormalArray = []
    ColorArray = []
    MaterialColor = []
    MaterialArray = []
    UVArray = []
    TBNArrayT = []
    TBNArrayB = []
    TBNArrayN = []
############################################################################









############################################################################
    def __init__(self):
        self.VertexArray = []
        self.NormalArray = []
        self.ColorArray = []
        self.MaterialColor = []
        self.MaterialArray = []
        self.UVArray = []
        self.TBNArrayT = []
        self.TBNArrayB = []
        self.TBNArrayN = []
############################################################################




############################################################################
    def read(self, mesh):
        self.VertexArray = self.readVertexArray(mesh)
        self.NormalArray = self.readNormalArray(mesh)
        self.ColorArray = self.readColorArray(mesh)
        self.MaterialColor = self.readMaterialColor(mesh)
        self.MaterialArray = self.readMaterialArray(mesh)
        self.UVArray = self.readUVArray(mesh)

        self.TBNArrayT, self.TBNArrayB, self.TBNArrayN = self.readTBNArray(self.VertexArray, self.UVArray, self.NormalArray, 0)


        if not self.VertexArray:
            print("VertexArray == None in " + str(mesh))

        if not self.NormalArray:
            print("NormalArray == None in " + str(mesh))

        if not self.ColorArray:
            print("ColorArray == None in " + str(mesh))

        if not self.MaterialArray:
            print("MaterialArray == None in " + str(mesh))

        if not self.TBNArrayT:
            print("TBNArrayT == None in " + str(mesh))

        if not self.TBNArrayB:
            print("TBNArrayB == None in " + str(mesh))

        if not self.TBNArrayN:
            print("TBNArrayN == None in " + str(mesh))


        return( self.VertexArray and
                self.NormalArray and
                self.ColorArray and
                self.MaterialArray and
                #self.UVArray and
                self.TBNArrayT and
                self.TBNArrayB and
                self.TBNArrayN )
############################################################################




############################################################################
    def readVertexArray(self, mesh):

        vertex_array = []

        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            faces = mesh.polygons
        else:
            faces = mesh.faces

        vertices = mesh.vertices

        cur_face = 0

        for f in faces:

            if len(f.vertices) == 3:
                vertex_array.append( Vector(vertices[f.vertices[0]].co) )
                vertex_array.append( Vector(vertices[f.vertices[1]].co) )
                vertex_array.append( Vector(vertices[f.vertices[2]].co) )

            elif len(f.vertices) == 4:
                vertex_array.append( Vector(vertices[f.vertices[0]].co) )
                vertex_array.append( Vector(vertices[f.vertices[1]].co) )
                vertex_array.append( Vector(vertices[f.vertices[2]].co) )
                vertex_array.append( Vector(vertices[f.vertices[0]].co) )
                vertex_array.append( Vector(vertices[f.vertices[2]].co) )
                vertex_array.append( Vector(vertices[f.vertices[3]].co) )

            else:
                print("Unhandled polygon type in %s, face: %d, num vertices: %d" % (mesh.name, cur_face, len(f.vertices)))


            cur_face += 1

        return vertex_array
############################################################################




############################################################################
    def readNormalArray(self, mesh):
        normal_array = []


        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            faces = mesh.polygons
        else:
            faces = mesh.faces

        vertices = mesh.vertices

        cur_face = 0

        for f in faces:

            if len(f.vertices) == 3:
                if f.use_smooth:
                    normal_array.append( Vector(vertices[f.vertices[0]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[1]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[2]].normal) )
                else:
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )


            elif len(f.vertices) == 4:
                if f.use_smooth:
                    normal_array.append( Vector(vertices[f.vertices[0]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[1]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[2]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[0]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[2]].normal) )
                    normal_array.append( Vector(vertices[f.vertices[3]].normal) )
                else:
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )
                    normal_array.append( Vector(f.normal) )

            else:
                print("Unhandled polygon type in %s, face: %d, num vertices: %d" % (mesh.name, cur_face, len(f.vertices)))

            cur_face += 1

        return normal_array
############################################################################




############################################################################
    def readMaterialColor(self, mesh):

        c = []

        if len(mesh.materials) > 0 and mesh.materials[0] :
            c.append( mesh.materials[0].diffuse_color.r )
            c.append( mesh.materials[0].diffuse_color.g )
            c.append( mesh.materials[0].diffuse_color.b )
            c.append( mesh.materials[0].alpha )

        else:
            c.append(1.0)
            c.append(1.0)
            c.append(1.0)
            c.append(1.0)

        return c
############################################################################




############################################################################
    def readColorArray(self, mesh):

        color_array = []

        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
            mesh.update(calc_tessface=True, calc_edges=True)
            vcs = mesh.tessface_vertex_colors
        else:
            vcs = mesh.vertex_colors

        if len(mesh.materials) > 0 and mesh.materials[0]  and  len(vcs) > 0 :
            if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:
                faces = mesh.polygons
            else:
                faces = mesh.faces

            vertices = mesh.vertices

            cur_face = 0

            for f in faces:
                if len(f.vertices) == 3:

                    color = vcs[0].data[ f.index ].color1
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color2
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color3
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                elif len(f.vertices) == 4:

                    color = vcs[0].data[ f.index ].color1
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color2
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color3
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color1
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color3
                    color_array.append( [color.r, color.g, color.b, 1.0] )


                    color = vcs[0].data[ f.index ].color4
                    color_array.append( [color.r, color.g, color.b, 1.0] )

                else:
                    print("Unhandled polygon type in %s, face: %d, num vertices: %d" % (mesh.name, cur_face, len(f.vertices)))

                cur_face += 1

        else:
            c = []
            c.append( mesh.materials[0].diffuse_color.r )
            c.append( mesh.materials[0].diffuse_color.g )
            c.append( mesh.materials[0].diffuse_color.b )
            c.append( mesh.materials[0].alpha )

            color_array.append(c)


        return color_array
############################################################################




############################################################################
    def readMaterialArray(self, mesh):

        material_array = []

        if len(mesh.materials) > 0 and mesh.materials[0] :
            material = []
            material.append( mesh.materials[0].ambient )
            material.append( mesh.materials[0].diffuse_intensity )
            material.append( mesh.materials[0].specular_intensity )
            material.append( mesh.materials[0].specular_hardness / 511.0 )
            material_array.append(material)

        else:
            material = []
            material.append( 1.0 )
            material.append( 1.0 )
            material.append( 0.0 )
            material.append( 0.0 )
            material_array.append(material)

        return material_array
############################################################################




############################################################################
    def readUVArray(self, mesh):
        uv_array = []



        if bpy.app.version[0] == 2 and bpy.app.version[1] >= 63:

            #if len(mesh.tessface_uv_textures) == 0:
            mesh.update(calc_tessface=True, calc_edges=True)

            faces = mesh.polygons
            uv_textures = mesh.tessface_uv_textures
        else:
            faces = mesh.faces
            uv_textures = mesh.uv_textures


        if len(uv_textures) == 0:
            return uv_array


        for i in range(0, len(uv_textures), 1):
            uv_array.append([])
        #uv_array.append([])


        cur_face = 0

        for f in faces:
            for t in range(0, len(uv_textures), 1):
            #t = 0
                tex = uv_textures[t]

                if f.index < len(tex.data):
                    uvface = tex.data[f.index]

                    if len(f.vertices) == 3:
                        #uv_array[t].append( (uvface.uv[0][0], uvface.uv[0][1]) )
                        #uv_array[t].append( (uvface.uv[1][0], uvface.uv[1][1]) )
                        #uv_array[t].append( (uvface.uv[2][0], uvface.uv[2][1]) )
                        uv_array[t].append( (uvface.uv1[0], uvface.uv1[1]) )
                        uv_array[t].append( (uvface.uv2[0], uvface.uv2[1]) )
                        uv_array[t].append( (uvface.uv3[0], uvface.uv3[1]) )

                    elif len(f.vertices) == 4:
                        #uv_array[t].append( (uvface.uv[0][0], uvface.uv[0][1]) )
                        #uv_array[t].append( (uvface.uv[1][0], uvface.uv[1][1]) )
                        #uv_array[t].append( (uvface.uv[2][0], uvface.uv[2][1]) )
                        uv_array[t].append( (uvface.uv1[0], uvface.uv1[1]) )
                        uv_array[t].append( (uvface.uv2[0], uvface.uv2[1]) )
                        uv_array[t].append( (uvface.uv3[0], uvface.uv3[1]) )

                        #uv_array[t].append( (uvface.uv[0][0], uvface.uv[0][1]) )
                        #uv_array[t].append( (uvface.uv[2][0], uvface.uv[2][1]) )
                        #uv_array[t].append( (uvface.uv[3][0], uvface.uv[3][1]) )
                        uv_array[t].append( (uvface.uv1[0], uvface.uv1[1]) )
                        uv_array[t].append( (uvface.uv3[0], uvface.uv3[1]) )
                        uv_array[t].append( (uvface.uv4[0], uvface.uv4[1]) )

                    else:
                        print("Unhandled polygon type in %s, face: %d, num vertices: %d" % (mesh.name, cur_face, len(f.vertices)))
                else:
                    pass


                cur_face += 1



        return uv_array
############################################################################




############################################################################
    def readTBNArray(self, vertex_array, uv_array, normal_array, normal_unit):
        T = []
        B = []
        N = []


        for i in range(0, len(vertex_array), 3):
            v0 = vertex_array[i]
            v1 = vertex_array[i+1]
            v2 = vertex_array[i+2]

            u0 = [0, 0]
            u1 = [0, 1]
            u2 = [1, 1]

            try:
                u0 = uv_array[normal_unit][i]
                u1 = uv_array[normal_unit][i+1]
                u2 = uv_array[normal_unit][i+2]
            except:
                pass

            t, b, n = self.computeTBN(v0, v1, v2, u0, u1, u2, normal_array[i])
            T.append(t)
            B.append(b)
            N.append(n)

            t, b, n = self.computeTBN(v1, v2, v0, u1, u2, u0, normal_array[i+1])
            T.append(t)
            B.append(b)
            N.append(n)

            t, b, n = self.computeTBN(v2, v0, v1, u2, u0, u1, normal_array[i+2])
            T.append(t)
            B.append(b)
            N.append(n)



        return T, B, N
############################################################################




############################################################################
    def computeTBN(self, p1, p2, p3, uv1, uv2, uv3, normal):


        u1 = uv1[0]
        u2 = uv2[0]
        u3 = uv3[0]

        v1 = uv1[1]
        v2 = uv2[1]
        v3 = uv3[1]

        T = Vector()
        B = Vector()
        N = Vector()


        #T = ( (p2 - p1) * (v3 - v1) - (p3 - p1) * (v2 - v1) ) / ( (u2 - u1) * (v3 - v1) - (v2 - v1) * (u3 - u1) )
        #B = ( (p2 - p1) * (u3 - u1) - (p3 - p1) * (u2 - u1) ) / ( (u3 - u1) * (v2 - v1) - (v3 - v1) * (u2 - u1) )
        T = (p2 - p1) * (v3 - v1) - (p3 - p1) * (v2 - v1)
        B = (p2 - p1) * (u3 - u1) - (p3 - p1) * (u2 - u1)

        #T.normalize()
        #B.normalize()
        #N = T.cross(B)

        N.x = normal.x
        N.y = normal.y
        N.z = normal.z

        T = N.cross(B)
        B = N.cross(T)

        T.normalize()
        B.normalize()
        N.normalize()

        return T, B, N



        #x0 = v1.x - v0.x
        #x1 = v2.x - v0.x

        #y0 = v1.y - v0.y
        #y1 = v2.y - v0.y

        #z0 = v1.z - v0.z
        #z1 = v2.z - v0.z



        #s0 = u1[0] - u0[0]
        #s1 = u2[0] - u0[0]

        #t0 = u1[1] - u0[1]
        #t1 = u2[1] - u0[1]



        #r = 1.0

        #try:
            #r = 1.0 / (s0 * t1 - s1 * t0)
        #except:
            #print("Zero division error in computeTBN")
            #r = 1.0

        #t = Vector()
        #b = Vector()
        #n = Vector()

        #t.x = r * (t1 * x0 - t0 * x1)
        #t.y = r * (t1 * y0 - t0 * y1)
        #t.z = r * (t1 * z0 - t0 * z1)

        #b.x = r * (s0 * x1 - s1 * x0)
        #b.y = r * (s0 * y1 - s1 * y0)
        #b.z = r * (s0 * z1 - s1 * z0)

        ##t.normalize()
        ##b.normalize()

        #n = normal

        #t = b.cross(n)
        #b = t.cross(n)

        #t.normalize()
        #b.normalize()



        #return t, b, n
############################################################################




# ........................................................................ #
############################################################################
