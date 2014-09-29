# -*- coding: iso-8859-1 -*-
# file Geode.py
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
from . import Writable
############################################################################




############################################################################
# ........................................................................ #
ALBEDO = 1
MATERIAL = 2
NORMAL = 4
SHADOW_CAST = 8
TRANSPARENCY = 16
TRANSPARENCY_IOR = 32
WATER = 64
ALWAYS_ON_TOP_AMN = 128
FOG = 256
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Geode(Writable.Writable):
    """Geode in the scene"""





############################################################################
    Object = None
    MeshData = None
    StateSet = None
    NodeMask = None
    ReadOK = None
    LOD = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Geode, self).__init__(data)

        self.Object = obj
        self.MeshData = None
        self.StateSet = None
        self.NodeMask = None
        self.LOD = None
        self.LODuid = None
############################################################################




############################################################################
    def buildGraph(self):
        super(Geode, self).buildGraph()

        mesh = self.Object.data


        self.NodeMask = ALBEDO | MATERIAL | NORMAL


        self.LOD = [0.0, 0.0]


        try:
            self.LOD[0] = self.Object["oo_lod_min"]
        except:
            self.LOD[0] = 0.0

        try:
            self.LOD[1] = self.Object["oo_lod_max"]
            self.LODuid = self.Data.UniqueID.generate() ;
        except:
            self.LOD = None
            self.LODuid = None



        if self.Data.Cache.has(mesh):
            self.MeshData = None
            self.StateSet = None
            self.UniqueID = self.Data.Cache.get(mesh)
            self.ReadOK = True

        else:
            from . import MeshData
            self.MeshData = MeshData.MeshData()
            read_ok = self.MeshData.read(mesh)

            self.ReadOK = read_ok

            if read_ok:
                self.Data.Cache.set(mesh, self.UniqueID)

            else:
                #self.Data.Operator.report({'ERROR'}, "[%s] Cannot read mesh" %mesh)
                #return False
                #return self.traverseBuild()
                pass


            if len(self.Object.data.materials) :
                if self.Object.data.materials[0]:
                    from . import StateSet
                    self.StateSet = StateSet.StateSet(self.Data, self.Object)
                    self.addChild(self.StateSet)


                    material = self.MeshData.MaterialArray[0]
                    material[0] *= self.Data.AmbientMultiplier

                    if self.Data.ExportLights:
                        self.StateSet.UniformList.addVec4Uniform("uMaterial", material)
                        self.StateSet.UniformList.addVec4Uniform("uColor", self.MeshData.MaterialColor)
                        self.StateSet.UniformList.addFloatUniform("uEmission", self.Object.data.materials[0].emit)

                        if self.Object.data.materials[0].subsurface_scattering.use:
                            self.StateSet.UniformList.addFloatUniform("uIOR", self.Object.data.materials[0].subsurface_scattering.ior)
                        else:
                            self.StateSet.UniformList.addFloatUniform("uIOR", 1.0)


                    if self.Object.data.materials[0].use_transparency:
                        if self.Object.data.materials[0].subsurface_scattering.use:
                            self.NodeMask = TRANSPARENCY_IOR
                        else:
                            self.NodeMask = TRANSPARENCY


                    if self.Object.data.materials[0].use_cast_shadows_only:
                        self.NodeMask = 0


                    if self.Object.data.materials[0].use_cast_buffer_shadows:
                        self.NodeMask |= SHADOW_CAST


                    try:
                        if self.Object.fog_enabled:
                            self.NodeMask = FOG


                            speed = [0, 0]

                            speed[0] = self.Object.fog_speed_x
                            speed[1] = self.Object.fog_speed_y

                            if self.Data.ExportLights:
                                self.StateSet.UniformList.addVec2Uniform("uFogSpeed", speed)
                    except:
                        pass


                    try:
                        if self.Object.water_enabled:
                            self.NodeMask = WATER
                    except:
                        pass


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):

        if self.LOD:
            writer.moveIn("osg::LOD")

            writer.writeLine("UniqueID %u" % self.LODuid)

            writer.moveIn("Children 1")
            writer.moveIn("osg::Geode")

            self.writeGeodeData(writer)

            writer.moveOut("osg::Geode")
            writer.moveOut("Children 1")


            writer.writeLine( "CenterMode USER_DEFINED_CENTER" )
            writer.writeLine( "UserCenter 0 0 0 -1" )

            writer.moveIn("RangeList 1")
            writer.writeLine("%f %f" % (self.LOD[0], self.LOD[1]))
            writer.moveOut("RangeList 1")

            writer.moveOut("osg::LOD")



        else:
            writer.moveIn("osg::Geode")

            self.writeGeodeData(writer)

            writer.moveOut("osg::Geode")


        return True
############################################################################




############################################################################
    def writeGeodeData(self, writer):

        if not super(Geode, self).writeToStream(writer) :
            return False


        if self.StateSet and self.MeshData:
            writer.writeLine("Name \"Geode@%s\"" %self.Object.name)
            writer.writeLine("NodeMask 0x%x" %self.NodeMask)



        if self.StateSet:
            self.StateSet.writeToStream(writer)




        if self.ReadOK and self.MeshData:


            writer.moveIn("Drawables 1") ;
            writer.moveIn("osg::Geometry") ;

            writer.writeLine("UniqueID %d" %self.Data.UniqueID.generate())


            writer.writeLine("Name \"Geometry@%s\"" %self.Object.name) ;

            writer.moveIn("PrimitiveSetList 1")
            writer.writeLine("DrawArrays GL_TRIANGLES 0 0 %d" % len(self.MeshData.VertexArray))
            writer.moveOut("PrimitiveSetList 1")




            writer.moveIn("VertexData")


            array_id = self.Data.VertexID.generate()
            array_len = len(self.MeshData.VertexArray)

            writer.moveIn("Array TRUE ArrayID %d Vec3fArray %d" %(array_id, array_len))

            for v in self.MeshData.VertexArray:
                writer.writeLine("%f %f %f" %(v.x, v.y, v.z))

            writer.moveOut("Array TRUE ArrayID %d Vec3fArray %d" %(array_id, array_len))


            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_PER_VERTEX")
            writer.writeLine("Normalize 0")


            writer.moveOut("VertexData")




            writer.moveIn("NormalData")


            array_id = self.Data.VertexID.generate()
            array_len = len(self.MeshData.NormalArray)

            writer.moveIn("Array TRUE ArrayID %d Vec3fArray %d" %(array_id, array_len))

            for v in self.MeshData.NormalArray:
                writer.writeLine("%f %f %f" %(v.x, v.y, v.z))

            writer.moveOut("Array TRUE ArrayID %d Vec3fArray %d" %(array_id, array_len))


            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_PER_VERTEX")
            writer.writeLine("Normalize 0")


            writer.moveOut("NormalData")




            writer.moveIn("ColorData")


            array_id = self.Data.VertexID.generate()

            num_elements = len(self.MeshData.ColorArray)

            writer.moveIn("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))

            for v in self.MeshData.ColorArray:
                writer.writeLine("%f %f %f %f" %(v[0], v[1], v[2], v[3]))

            writer.moveOut("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))


            writer.writeLine("Indices FALSE")

            if num_elements == 1 :
                writer.writeLine("Binding BIND_OVERALL")
            else:
                writer.writeLine("Binding BIND_PER_VERTEX")

            writer.writeLine("Normalize 0")


            writer.moveOut("ColorData")




            if self.MeshData.UVArray and self.MeshData.UVArray != []:

                num_tex = len(self.MeshData.UVArray)

                #writer.moveIn("TexCoordData %d" %num_tex)
                writer.moveIn("TexCoordData 3")

                for uv_array in self.MeshData.UVArray:

                #uv_array = self.MeshData.UVArray[0]

                #if True:

                    writer.moveIn("Data") ;

                    array_id = self.Data.VertexID.generate()
                    num_elements = len(uv_array)

                    writer.moveIn("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))

                    for uv in uv_array:
                        writer.writeLine("%f %f" %(uv[0], uv[1]))

                    writer.moveOut("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))


                    writer.writeLine("Indices FALSE")
                    writer.writeLine("Binding BIND_PER_VERTEX")
                    writer.writeLine("Normalize 0")

                    writer.moveOut("Data") ;



                if len(self.MeshData.UVArray) < 3:
                    uv_array = self.MeshData.UVArray[0]

                    writer.moveIn("Data") ;

                    array_id = self.Data.VertexID.generate()
                    num_elements = len(uv_array)

                    writer.moveIn("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))

                    for uv in uv_array:
                        writer.writeLine("%f %f" %(uv[0], uv[1]))

                    writer.moveOut("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))


                    writer.writeLine("Indices FALSE")
                    writer.writeLine("Binding BIND_PER_VERTEX")
                    writer.writeLine("Normalize 0")

                    writer.moveOut("Data") ;



                if len(self.MeshData.UVArray) < 2:
                    uv_array = self.MeshData.UVArray[0]

                    writer.moveIn("Data") ;

                    array_id = self.Data.VertexID.generate()
                    num_elements = len(uv_array)

                    writer.moveIn("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))

                    for uv in uv_array:
                        writer.writeLine("%f %f" %(uv[0], uv[1]))

                    writer.moveOut("Array TRUE ArrayID %d Vec2fArray %d" %(array_id, num_elements))


                    writer.writeLine("Indices FALSE")
                    writer.writeLine("Binding BIND_PER_VERTEX")
                    writer.writeLine("Normalize 0")

                    writer.moveOut("Data") ;



                #writer.moveOut("TexCoordData %d" %num_tex)
                writer.moveOut("TexCoordData 3")





            writer.moveIn("VertexAttribData 8")




            writer.moveIn("Data")
            writer.writeLine("Array FALSE")
            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_OFF")
            writer.writeLine("Normalize 0")
            writer.moveOut("Data")




            if self.MeshData.TBNArrayT and self.MeshData.TBNArrayT != []:
                writer.moveIn("Data") ;

                array_id = self.Data.VertexID.generate()
                num_elements = len(self.MeshData.TBNArrayT)

                writer.moveIn("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))

                for t in self.MeshData.TBNArrayT:
                    writer.writeLine("%f %f %f %f" %(t.x, t.y, t.z, 0.0))

                writer.moveOut("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))


                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_PER_VERTEX")
                writer.writeLine("Normalize 0")

                writer.moveOut("Data") ;


            else:
                writer.moveIn("Data")
                writer.writeLine("Array FALSE")
                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_OFF")
                writer.writeLine("Normalize 0")
                writer.moveOut("Data")



            writer.moveIn("Data")
            writer.writeLine("Array FALSE")
            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_OFF")
            writer.writeLine("Normalize 0")
            writer.moveOut("Data")



            writer.moveIn("Data")
            writer.writeLine("Array FALSE")
            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_OFF")
            writer.writeLine("Normalize 0")
            writer.moveOut("Data")



            writer.moveIn("Data")
            writer.writeLine("Array FALSE")
            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_OFF")
            writer.writeLine("Normalize 0")
            writer.moveOut("Data")



            writer.moveIn("Data")
            writer.writeLine("Array FALSE")
            writer.writeLine("Indices FALSE")
            writer.writeLine("Binding BIND_OFF")
            writer.writeLine("Normalize 0")
            writer.moveOut("Data")




            if self.MeshData.TBNArrayB and self.MeshData.TBNArrayB != []:
                writer.moveIn("Data") ;

                array_id = self.Data.VertexID.generate()
                num_elements = len(self.MeshData.TBNArrayB)

                writer.moveIn("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))

                for t in self.MeshData.TBNArrayB:
                    writer.writeLine("%f %f %f %f" %(t.x, t.y, t.z, 0.0))

                writer.moveOut("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))


                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_PER_VERTEX")
                writer.writeLine("Normalize 0")

                writer.moveOut("Data") ;
            else:
                writer.moveIn("Data")
                writer.writeLine("Array FALSE")
                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_OFF")
                writer.writeLine("Normalize 0")
                writer.moveOut("Data")




            if self.MeshData.TBNArrayN and self.MeshData.TBNArrayN != []:
                writer.moveIn("Data") ;

                array_id = self.Data.VertexID.generate()
                num_elements = len(self.MeshData.TBNArrayN)

                writer.moveIn("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))

                for t in self.MeshData.TBNArrayN:
                    writer.writeLine("%f %f %f %f" %(t.x, t.y, t.z, 0.0))

                writer.moveOut("Array TRUE ArrayID %d Vec4fArray %d" %(array_id, num_elements))


                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_PER_VERTEX")
                writer.writeLine("Normalize 0")

                writer.moveOut("Data") ;
            else:
                writer.moveIn("Data")
                writer.writeLine("Array FALSE")
                writer.writeLine("Indices FALSE")
                writer.writeLine("Binding BIND_OFF")
                writer.writeLine("Normalize 0")
                writer.moveOut("Data")





            writer.moveOut("VertexAttribData 8")





            writer.moveOut("osg::Geometry") ;
            writer.moveOut("Drawables 1") ;


        # endif self.MeshData


        return True
############################################################################




# ........................................................................ #
############################################################################
