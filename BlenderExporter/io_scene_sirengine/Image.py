# -*- coding: iso-8859-1 -*-
# file Image.py
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

import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Image(Writable.Writable):
    """Image in the scene"""





############################################################################
    Img = None
    FilePath = None
############################################################################









############################################################################
    def __init__(self, data, image):
        super(Image, self).__init__(data)

        self.Img = image
############################################################################




############################################################################
    def buildGraph(self):
        super(Image, self).buildGraph()

        if self.Data.Cache.has(self.Img):
            self.UniqueID = self.Data.Cache.get(self.Img)
            self.Img = None

        else:
            self.FilePath = bpy.path.relpath(self.Img.filepath).replace("//", "").replace("\\", "/")
            self.Data.Cache.set(self.Img, self.UniqueID)


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("Image TRUE") ;

        if not super(Image, self).writeToStream(writer) :
            return False


        if self.Img:
            writer.writeLine("FileName \"%s\"" %self.FilePath)
            writer.writeLine("WriteHint 0 2")
            writer.writeLine("DataVariance STATIC")


        writer.moveOut("Image TRUE")

        return True
############################################################################




# ........................................................................ #
############################################################################
