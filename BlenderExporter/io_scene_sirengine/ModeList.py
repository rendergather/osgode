# -*- coding: iso-8859-1 -*-
# file ModeList.py
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
class ModeList(Writable.Writable):
    """ModeList in a StateSet"""





############################################################################
    Modes = []
############################################################################









############################################################################
    def __init__(self, data):
        super(ModeList, self).__init__(data)

        self.Modes = []
############################################################################




############################################################################
    def addMode(self, mode):
        self.Modes.append(mode)
############################################################################




############################################################################
    def buildGraph(self):
        super(ModeList, self).buildGraph()


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        num_modes = len(self.Modes)

        if num_modes:
            writer.moveIn("ModeList %d" % num_modes)

            for mode in self.Modes:
                writer.writeLine(mode)


            writer.moveOut("ModeList %d" % num_modes)

        return True
############################################################################




# ........................................................................ #
############################################################################
