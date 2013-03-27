# -*- coding: iso-8859-1 -*-
# file TextureAttribute.py
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
class TextureAttribute(Writable.Writable):
    """TextureAttribute in the scene"""





############################################################################
    Texture = None
    Image = None
    Wrap = "REPEAT"
    MaxAnisotropy = 1
############################################################################









############################################################################
    def __init__(self, data, texture):
        super(TextureAttribute, self).__init__(data)

        self.Texture = texture
        self.Image = None
        self.Wrap = "REPEAT"
        self.MaxAnisotropy = 16
############################################################################




############################################################################
    def buildGraph(self):
        super(TextureAttribute, self).buildGraph()

        if self.Data.Cache.has(self.Texture.texture):
            self.UniqueID = self.Data.Cache.get(self.Texture.texture)
            self.Texture = None

        else:
            if self.Texture.texture.extension == "REPEAT":
                self.Wrap = "REPEAT"
            else:
                self.Wrap = "CLAMP"

            self.Data.Cache.set(self.Texture.texture, self.UniqueID)


            from . import Image
            self.Image = Image.Image(self.Data, self.Texture.texture.image)
            self.addChild(self.Image)

        self.MaxAnisotropy = 16


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Texture2D") ;

        if not super(TextureAttribute, self).writeToStream(writer) :
            return False


        if self.Texture:
            writer.writeLine("WRAP_S %s" %self.Wrap)
            writer.writeLine("WRAP_T %s" %self.Wrap)
            writer.writeLine("WRAP_R %s" %self.Wrap)
            writer.writeLine("MIN_FILTER LINEAR_MIPMAP_LINEAR")
            writer.writeLine("MAG_FILTER LINEAR")

            writer.writeLine("MaxAnisotropy %d" %self.MaxAnisotropy)


            if self.Image:
                if not self.Image.writeToStream(writer):
                    return False


        writer.moveOut("osg::Texture2D")

        return True
############################################################################




# ........................................................................ #
############################################################################
