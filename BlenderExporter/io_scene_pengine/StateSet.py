# -*- coding: iso-8859-1 -*-
# file StateSet.py
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class StateSet(Writable.Writable):
    """StateSet in the scene"""





############################################################################
    Object = None
    TextureModeList = None
    TextureAttributeList = None
    ModeList = None
    UniformList = None
    Transparent = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(StateSet, self).__init__(data)

        self.Object = obj
        self.TextureModeList = None
        self.TextureAttributeList = None
        self.ModeList = None
        self.UniformList = None
        self.Transparent = None

        from . import ModeList
        self.ModeList = ModeList.ModeList(self.Data)

        from . import UniformList
        self.UniformList = UniformList.UniformList(self.Data)
############################################################################




############################################################################
    def buildGraph(self):
        super(StateSet, self).buildGraph()


        if self.Object:

            mesh = self.Object.data
            materials = mesh.materials


            cull_face = "GL_CULL_FACE ON"

            if self.Object.data.show_double_sided:
                cull_face = "GL_CULL_FACE OFF"

            if not cull_face in self.Data.MasterStateSet.ModeList.Modes:
                self.ModeList.addMode(cull_face)




            self.ModeList.addMode("GL_NORMALIZE ON")




            if self.Object.data.materials[0]  and self.Object.data.materials[0].use_transparency:
                self.ModeList.addMode("GL_BLEND ON")
                self.Transparent = True

            if len(materials):
                material = materials[0]

                if material:

                    from . import TextureModeList
                    from . import TextureAttributeList

                    if material.texture_slots and len(material.texture_slots):
                        self.TextureModeList = TextureModeList.TextureModeList(self.Data, material.texture_slots)
                        self.addChild( self.TextureModeList )

                        self.TextureAttributeList = TextureAttributeList.TextureAttributeList(self.Data, material.texture_slots)
                        self.addChild( self.TextureAttributeList )


                        height_map = material.texture_slots[1]

                        if height_map:
                            if height_map.use_map_displacement:
                                self.UniformList.addFloatUniform("uParallaxScale", height_map.displacement_factor)


        self.addChild( self.UniformList )


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):

        if  len(self.ModeList.Modes) == 0 and \
            len(self.TextureModeList.Modes) == 0 and \
            len(self.TextureAttributeList.TextureAttributes) == 0 :


            return True




        writer.moveIn("StateSet TRUE") ;
        writer.moveIn("osg::StateSet") ;

        if not super(StateSet, self).writeToStream(writer) :
            return False


        if self.ModeList:
            self.ModeList.writeToStream(writer)

        if self.TextureModeList:
            self.TextureModeList.writeToStream(writer)

        if self.TextureAttributeList:
            self.TextureAttributeList.writeToStream(writer)

        if self.UniformList:
            self.UniformList.writeToStream(writer)


        if self.Transparent == True:
            writer.writeLine("RenderingHint 2")
            writer.writeLine("RenderBinMode USE_RENDERBIN_DETAILS")
            writer.writeLine("BinNumber 10")
            writer.writeLine("BinName \"DepthSortedBin\"")


        writer.moveOut("osg::StateSet")
        writer.moveOut("StateSet TRUE")

        return True
############################################################################




# ........................................................................ #
############################################################################
