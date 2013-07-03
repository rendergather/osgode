# -*- coding: iso-8859-1 -*-
# file TextureModeList.py
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
class TextureModeList(Writable.Writable):
    """TextureModeList in the scene"""





############################################################################
    Slots = None
    Modes = []
############################################################################









############################################################################
    def __init__(self, data, slots):
        super(TextureModeList, self).__init__(data)

        self.Slots = slots
        self.Modes = []
############################################################################




############################################################################
    def buildGraph(self):
        super(TextureModeList, self).buildGraph()


        for slot in self.Slots:
            if slot:
                if slot.use_map_color_diffuse:
                    self.Modes.append(True)
                else:
                    self.Modes.append(False)


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        num_modes = len(self.Modes)


        if num_modes:

            writer.moveIn("TextureModeList %d" % num_modes)

            for i in self.Modes:
                writer.moveIn("Data 1")

                if i:
                    writer.writeLine("GL_TEXTURE_2D ON")
                else:
                    writer.writeLine("GL_TEXTURE_2D OFF")

                writer.moveOut("Data 1")

            writer.moveOut("TextureModeList %d" % num_modes)

        return True
############################################################################




# ........................................................................ #
############################################################################
