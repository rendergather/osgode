# -*- coding: iso-8859-1 -*-
# file NearCallback.py
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
class NearCallback(Writable.Writable):
    """osgODE::NearCallback"""





############################################################################
    CollisionParameters = None
############################################################################









############################################################################
    def __init__(self, data):
        super(NearCallback, self).__init__(data)
        self.CollisionParameters = None
############################################################################




############################################################################
    def buildGraph(self):
        super(NearCallback, self).buildGraph()

        from . import CollisionParameters
        self.CollisionParameters = CollisionParameters.CollisionParameters(self.Data)
        return self.CollisionParameters.buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osgODE::DefaultNearCallback") ;

        if not super(NearCallback, self).writeToStream(writer) :
            return False


        writer.moveIn("CollisionParameters TRUE")

        if not self.CollisionParameters.writeToStream(writer):
            return False

        writer.moveOut("CollisionParameters TRUE")



        writer.moveOut("osgODE::DefaultNearCallback")

        return True
############################################################################




# ........................................................................ #
############################################################################
