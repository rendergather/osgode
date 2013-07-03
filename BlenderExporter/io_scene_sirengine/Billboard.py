# -*- coding: iso-8859-1 -*-
# file Billboard.py
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
from . import Geode
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Billboard(Geode.Geode):
    """Billboard in the scene"""





############################################################################
    Axis = None
    Normal = None
    Mode = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Billboard, self).__init__(data, obj)

        self.Axis = None
        self.Normal = None
        self.Mode = None
############################################################################




############################################################################
    def buildGraph(self):
        

        self.Axis = [0, 0, 1]
        self.Normal = [0, 1, 0]
        self.Mode = "AXIAL_ROT"

        splitted = self.Object.name.split('.')

        if "PRE" in splitted:
            self.Mode = "POINT_ROT_EYE"
        elif "PRW" in splitted:
            self.Mode = "POINT_ROT_WORLD"


        return super(Billboard, self).buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Billboard")



        if not super(Billboard, self).writeGeodeData(writer) :
            return False



        writer.writeLine("Mode %s" %self.Mode)
        writer.writeLine("Axis %f %f %f" %(self.Axis[0], self.Axis[1], self.Axis[2]))
        writer.writeLine("Normal %f %f %f" %(self.Normal[0], self.Normal[1], self.Normal[2]))

        writer.moveIn("PositionList 1")
        writer.writeLine("0 0 0")
        writer.moveOut("PositionList 1")



        writer.moveOut("osg::Billboard")


        return True
############################################################################




# ........................................................................ #
############################################################################
