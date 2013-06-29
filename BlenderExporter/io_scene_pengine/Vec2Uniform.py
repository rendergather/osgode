# -*- coding: iso-8859-1 -*-
# file Vec2Uniform.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2013 by Rocco Martino                                   #
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
class Vec2Uniform(Writable.Writable):
    """Vec2Uniform in the scene"""





############################################################################
    Name = None
    Value = None
    ArrayID = None
############################################################################









############################################################################
    def __init__(self, data, uni_name, uni_val):
        super(Vec2Uniform, self).__init__(data)

        self.Name = uni_name
        self.Value = uni_val
        self.ArrayID = None
############################################################################




############################################################################
    def buildGraph(self):
        super(Vec2Uniform, self).buildGraph()


        self.ArrayID = self.Data.VertexID.generate()


        return True

        #return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Uniform") ;

        if not super(Vec2Uniform, self).writeToStream(writer) :
            return False



        writer.writeLine("Name \"%s\"" %self.Name)
        writer.writeLine("Type FLOAT_VEC2")
        writer.writeLine("NumElements 1")

        writer.moveIn("Elements TRUE ArrayID %d FloatArray 2" %self.ArrayID)
        writer.writeLine("%f %f" %(self.Value[0], self.Value[1]))
        writer.moveOut("Elements TRUE ArrayID %d FloatArray 2" %self.ArrayID)


        writer.moveOut("osg::Uniform")

        return True
############################################################################




# ........................................................................ #
############################################################################
