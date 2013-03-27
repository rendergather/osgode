# -*- coding: iso-8859-1 -*-
# file ODEObject.py
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
class ODEObject(Writable.Writable):
    """osgODE::ODEObject"""





############################################################################
    Object = None
    ID = 0
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(ODEObject, self).__init__(data)

        self.Object = obj
        self.ID = 0
############################################################################




############################################################################
    def buildGraph(self):
        super(ODEObject, self).buildGraph()

        self.ID = self.Data.ODEID.generate()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::ODEObject") ;

        self.writePrivateData(writer)

        writer.moveOut("osgODE::ODEObject")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(ODEObject, self).writeToStream(writer) :
            return False


        writer.writeLine("Name \"%s@%s\"" %(self.__class__.__name__, self.Object.name))


        writer.writeLine("ID %s" %self.ID)


        return True
############################################################################




# ........................................................................ #
############################################################################
