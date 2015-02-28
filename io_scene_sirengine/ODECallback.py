# -*- coding: iso-8859-1 -*-
# file ODECallback.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2014 by Rocco Martino                                   #
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
class ODECallback(Writable.Writable):
    """osgODE::ODECallback"""





############################################################################
    Object = None
    NestedCallback = None
    Name = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(ODECallback, self).__init__(data)

        self.Object = obj
        self.NestedCallback = None
        self.Name = None
############################################################################




############################################################################
    def buildGraph(self):
        super(ODECallback, self).buildGraph()


        self.Name = "%s@%s" %(self.__class__.__name__, self.Object.name)



        return True
############################################################################




############################################################################
    def addNestedCallback(self, cbk):
        if self.NestedCallback:
            self.NestedCallback.addNestedCallback( cbk )
        else:
            self.NestedCallback = cbk
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::ODECallback") ;

        self.writePrivateData(writer)

        writer.moveOut("osgODE::ODECallback")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(ODECallback, self).writeToStream(writer) :
            return False


        if self.Name:
            writer.writeLine("Name \"%s\"" %(self.Name))


        if self.NestedCallback:
            writer.moveIn("NestedCallback TRUE")
            self.NestedCallback.writeToStream(writer)
            writer.moveOut("NestedCallback TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################
