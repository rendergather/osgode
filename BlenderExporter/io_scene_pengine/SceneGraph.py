# -*- coding: iso-8859-1 -*-
# file SceneGraph.py
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
from . import Root
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class SceneGraph(object):
    """The exported scene graph"""





############################################################################
    Data = None
    Root = None
############################################################################









############################################################################
    def __init__(self, data):
        self.Data = data
############################################################################




############################################################################
    def checkObjects(self):
        return True
############################################################################




############################################################################
    def buildGraph(self):

        self.Root = Root.Root(self.Data)
        return self.Root.buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):

        if not self.Data.ExportSelected:
            return self.Root.writeToStream(writer)
        else:
            if self.Data.Selected:
                return self.Data.Selected.writeToStream(writer)

        return False
############################################################################




# ........................................................................ #
############################################################################
