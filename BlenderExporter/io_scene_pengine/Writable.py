# -*- coding: iso-8859-1 -*-
# file Writable.py
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Writable(object):
    """Root group in the scene"""





############################################################################
    Data = None
    UniqueID = -1
    Children = []
############################################################################









############################################################################
    def __init__(self, data):
        self.Data = data
        self.Children = []
        self.UniqueID = -1
############################################################################




############################################################################
    def addChild(self, child):
        self.Children.append(child)
############################################################################




############################################################################
    def traverseBuild(self):
        for child in self.Children:
            if not child.buildGraph():
                return False

        return True
############################################################################




############################################################################
    def traverseWrite(self, writer):
        for child in self.Children:
            if not child.writeToStream(writer):
                return False

        return True
############################################################################




############################################################################
    def buildGraph(self):
        self.UniqueID = self.Data.UniqueID.generate()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.writeLine( "UniqueID %s" % str(self.UniqueID) )
        return True
############################################################################




# ........................................................................ #
############################################################################
