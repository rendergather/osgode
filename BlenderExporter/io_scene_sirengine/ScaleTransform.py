# -*- coding: iso-8859-1 -*-
# file ScaleTransform.py
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

from mathutils import Matrix
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class ScaleTransform(Writable.Writable):
    """ScaleTransform"""





############################################################################
    Object = None
    Matrix = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(ScaleTransform, self).__init__(data)

        self.Object = obj
        self.Matrix = None
############################################################################




############################################################################
    def buildGraph(self):
        super(ScaleTransform, self).buildGraph()

        self.Matrix = Matrix(self.Object.matrix_world)


        if not self.Object.hide_render:
            from . import Geode
            from . import Billboard

            try:
                if self.Object.data.materials[0].game_settings.face_orientation == "BILLBOARD":
                    billboard = Billboard.Billboard(self.Data, self.Object)
                    self.addChild(billboard)
                else:
                    geode = Geode.Geode(self.Data, self.Object)
                    self.addChild(geode)
            except:
                    geode = Geode.Geode(self.Data, self.Object)
                    self.addChild(geode)



        #for child in self.Object.children:
            #if child.type == "LAMP":
                #if self.Data.ExportLights:
                    #from . import Light
                    #light = Light.Light(self.Data, child)
                    #self.addChild(light)


            #elif child.type == "MESH":
                #from . import MatrixTransform
                #matrix_transform = MatrixTransform.MatrixTransform(self.Data, child)
                #self.addChild(matrix_transform)


            #elif child.type == "SPEAKER":
                #if self.Data.ExportSounds:
                    #from . import Speaker
                    #speaker = Speaker.Speaker(self.Data, child)
                    #self.addChild(speaker)


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::MatrixTransform")

        if not super(ScaleTransform, self).writeToStream(writer) :
            return False


        num_children = len(self.Children)

        if num_children > 0 :
            writer.moveIn("Children %s" % num_children)

            if not self.traverseWrite(writer):
                return False

            writer.moveOut("Children %s" % num_children)



        scale = self.Matrix.decompose()[2]

        if scale[0] != 1 or scale[1] != 1 or scale[2] != 1:

            writer.moveIn("Matrix") ;

            writer.writeLine("%f 0  0  0" %scale[0])
            writer.writeLine("0  %f 0  0" %scale[1])
            writer.writeLine("0  0  %f 0" %scale[2])
            writer.writeLine("0  0  0  1")

            writer.moveOut("Matrix")




        writer.moveOut("osg::MatrixTransform")

        return True
############################################################################




# ........................................................................ #
############################################################################
