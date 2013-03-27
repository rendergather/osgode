# -*- coding: iso-8859-1 -*-
# file UniformList.py
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
from . import Vec4Uniform
from . import FloatUniform
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class UniformList(Writable.Writable):
    """UniformList in a StateSet"""





############################################################################
    Uniforms = []
############################################################################









############################################################################
    def __init__(self, data):
        super(UniformList, self).__init__(data)

        self.Uniforms = []
############################################################################




############################################################################
    def addVec4Uniform(self, uniform_name, vec4):
        self.Uniforms.append( Vec4Uniform.Vec4Uniform(self.Data, uniform_name, vec4) )
############################################################################




############################################################################
    def addFloatUniform(self, uniform_name, float_value):
        self.Uniforms.append( FloatUniform.FloatUniform(self.Data, uniform_name, float_value) )
############################################################################




############################################################################
    def buildGraph(self):
        super(UniformList, self).buildGraph()

        for uniform in self.Uniforms:
            uniform.buildGraph()


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        num_uniforms = len(self.Uniforms)

        if num_uniforms:
            writer.moveIn("UniformList %d" % num_uniforms)

            for uniform in self.Uniforms:
                uniform.writeToStream(writer)
                writer.writeLine("Value OFF")


            writer.moveOut("UniformList %d" % num_uniforms)

        return True
############################################################################




# ........................................................................ #
############################################################################
