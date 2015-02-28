# -*- coding: iso-8859-1 -*-
# file Cylinder.py
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
from . import Collidable
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Cylinder(Collidable.Collidable):
    """osgODE::Cylinder"""





############################################################################
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Cylinder, self).__init__(data, obj)
############################################################################




############################################################################
    def buildGraph(self):
        if not super(Cylinder, self).buildGraph():
            return False



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::Cylinder") ;

        if not self.writePrivateData(writer):
            return False

        writer.moveOut("osgODE::Cylinder")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Cylinder, self).writePrivateData(writer) :
            return False


        return True
############################################################################




# ........................................................................ #
############################################################################
