# -*- coding: iso-8859-1 -*-
# file Manager.py
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
from . import Space

import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Manager(Writable.Writable):
    """osgODE::Manager"""





############################################################################
    Space = None
    StepSize = 0.0167
############################################################################









############################################################################
    def __init__(self, data):
        super(Manager, self).__init__(data)

        self.Space = None
        self.StepSize = 0.0167
############################################################################




############################################################################
    def buildGraph(self):
        super(Manager, self).buildGraph()


        from . import Space

        self.Space = Space.Space(self.Data)


        self.StepSize = 1.0 / self.Data.Scene.game_settings.fps


        return self.Space.buildGraph()
############################################################################




############################################################################
    def writeToStream(self, writer):

        class_name = "osgODE::Manager"

        if self.Data.Threaded:
            class_name = "osgODE::ThreadedManager"

        writer.moveIn(class_name) ;

        if not super(Manager, self).writeToStream(writer) :
            return False


        writer.moveIn("UpdateCallback TRUE")

        if self.Data.Threaded:
            writer.moveIn("osgODE::ThreadedManagerUpdateCallback")
            writer.writeLine("UniqueID %d" % self.Data.UniqueID.generate() )
            writer.moveOut("osgODE::ThreadedManagerUpdateCallback")

        else:
            writer.moveIn("osgODE::ManagerUpdateCallback")
            writer.writeLine("UniqueID %d" % self.Data.UniqueID.generate() )
            writer.writeLine("MaxStepSize 0.1")
            writer.moveOut("osgODE::ManagerUpdateCallback")


        writer.moveOut("UpdateCallback TRUE")


        if self.Data.ExportGame:
            writer.moveIn("EventCallback TRUE")
            writer.moveIn("osgODE::ManagerEventHandler")
            writer.writeLine("UniqueID %d" % self.Data.UniqueID.generate() )
            writer.moveOut("osgODE::ManagerEventHandler")
            writer.moveOut("EventCallback TRUE")


        writer.writeLine("StepSize %f" %(self.StepSize))

        writer.writeLine("AcceptVisitors TRUE")


        writer.moveIn("World TRUE")

        if not self.Space.writeToStream(writer):
            return False

        writer.moveOut("World TRUE")



        writer.moveOut(class_name)

        return True
############################################################################




# ........................................................................ #
############################################################################
