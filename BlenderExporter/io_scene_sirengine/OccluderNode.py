# -*- coding: iso-8859-1 -*-
# file OccluderNode.py
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
from . import ConvexPlanarOccluder
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class OccluderNode(Writable.Writable):
    """OccluderNode in the scene"""





############################################################################
    Object = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(OccluderNode, self).__init__(data)

        self.Object = obj
############################################################################




############################################################################
    def buildGraph(self):
        super(OccluderNode, self).buildGraph()

        cpo = ConvexPlanarOccluder.ConvexPlanarOccluder(self.Data, self.Object)

        self.addChild(cpo)


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::OccluderNode") ;

        if not super(OccluderNode, self).writeToStream(writer) :
            return False



        writer.moveIn("Occluder TRUE")

        if not self.traverseWrite(writer):
            return False

        writer.moveOut("Occluder TRUE")



        writer.moveOut("osg::OccluderNode")


        return True
############################################################################




# ........................................................................ #
############################################################################
