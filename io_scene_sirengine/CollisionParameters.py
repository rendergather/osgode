# -*- coding: iso-8859-1 -*-
# file CollisionParameters.py
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
class CollisionParameters(Writable.Writable):
    """osgODE::CollisionParameters"""





############################################################################
    Mode = 12314
    Mu = 1.0
    Mu2 = 1.0
    SoftERP = 0.2
    SoftCFM = 1.0e-5
    MaxContactNum = 16
############################################################################









############################################################################
    def __init__(self, data):
        super(CollisionParameters, self).__init__(data)

        self.Mode = 12314
        self.Mu = 1.0
        self.Mu2 = 1.0
        self.Bounce = 0.0
        self.SoftERP = self.Data.StopERP
        self.SoftCFM = self.Data.StopCFM
        self.MaxContactNum = self.Data.MaxContactNum
############################################################################




############################################################################
    def buildGraph(self):
        super(CollisionParameters, self).buildGraph()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osgODE::CollisionParameters") ;

        if not super(CollisionParameters, self).writeToStream(writer) :
            return False

        if self.Bounce != 0.0:
            self.Mode = self.Mode + 4

        if self.Mode != 0:
            writer.writeLine("Mode %d"              %self.Mode)


        if self.Mu != 1.0:
            writer.writeLine("Mu %f"                %self.Mu)


        if self.Mu2 != 1.0:
            writer.writeLine("Mu2 %f"               %self.Mu2)


        if self.Bounce != 0.0:
            writer.writeLine("Bounce %f"            %self.Bounce)


        writer.writeLine("SoftERP %f"           %self.SoftERP)
        writer.writeLine("SoftCFM %f"           %self.SoftCFM)
        writer.writeLine("MaxContactNum %d"     %self.MaxContactNum)



        writer.moveOut("osgODE::CollisionParameters")

        return True
############################################################################




# ........................................................................ #
############################################################################
