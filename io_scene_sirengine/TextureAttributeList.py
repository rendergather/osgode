# -*- coding: iso-8859-1 -*-
# file TextureAttributeList.py
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
class TextureAttributeList(Writable.Writable):
    """TextureAttributeList in the scene"""





############################################################################
    Slots = None
    TextureAttributes = []
############################################################################









############################################################################
    def __init__(self, data, slots):
        super(TextureAttributeList, self).__init__(data)

        self.Slots = slots
        self.TextureAttributes = []
############################################################################




############################################################################
    def buildGraph(self):
        super(TextureAttributeList, self).buildGraph()


        from . import TextureAttribute

        for slot in self.Slots:
            if slot and slot.texture:
                texture = TextureAttribute.TextureAttribute(self.Data, slot)
                self.TextureAttributes.append(texture)
                self.addChild(texture)


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        num_attributes = len(self.TextureAttributes)


        if num_attributes:

            writer.moveIn("TextureAttributeList %d" % num_attributes)

            for attribute in self.TextureAttributes:
                writer.moveIn("Data 1")

                if not attribute.writeToStream(writer):
                    return False

                writer.writeLine("Value OFF")

                writer.moveOut("Data 1")

            writer.moveOut("TextureAttributeList %d" % num_attributes)

        return True
############################################################################




# ........................................................................ #
############################################################################
