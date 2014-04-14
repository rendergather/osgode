# -*- coding: iso-8859-1 -*-
# file SoundGroup.py
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
class SoundGroup(Writable.Writable):
    """SoundGroup group in the scene"""





############################################################################
############################################################################









############################################################################
    def __init__(self, data):
        super(SoundGroup, self).__init__(data)
############################################################################




############################################################################
    def buildGraph(self):
        super(SoundGroup, self).buildGraph()


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Group") ;

        if not super(SoundGroup, self).writeToStream(writer) :
            return False


        writer.writeLine("Name \"SoundGroup\"")


        num_children = len(self.Children)

        if num_children > 0 :
            writer.moveIn("Children %s" % num_children)

            if not self.traverseWrite(writer):
                return False

            writer.moveOut("Children %s" % num_children)


        writer.moveOut("osg::Group")

        return True
############################################################################




# ........................................................................ #
############################################################################
