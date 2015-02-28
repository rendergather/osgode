# -*- coding: iso-8859-1 -*-
# file Controller.py
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
from . import Writable, Actuator
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Controller(Writable.Writable):
    """ooGame::Controller"""





############################################################################
    Object = None
    BlenderController = None
    Cached = None
    StateIndex = None
    ActuatorList = None
    Operator = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(Controller, self).__init__(rigid_body.Data)

        self.RigidBody = rigid_body
        self.Object = rigid_body.Object
        self.BlenderController = sensor
        self.Cached = False
        self.StateIndex = 0
        self.ActuatorList = []
        self.Operator = 'USER_DEFINED'
############################################################################




############################################################################
    def buildGraph(self):
        super(Controller, self).buildGraph()

        if self.Data.Cache.has( self.BlenderController ):
            self.UniqueID = self.Data.Cache.get( self.BlenderController )
            self.Cached = True
        else:
            self.Data.Cache.set( self.BlenderController, self.UniqueID )
            self.Cached = False

            if "LOGIC_" in self.BlenderController.type:
                self.Operator = self.BlenderController.type.replace("LOGIC_", "")
            else:
                self.Operator = "USER_DEFINED"



            self.StateIndex = self.BlenderController.states - 1



            for a in self.BlenderController.actuators:
                self.ActuatorList.append( self.Object.game.actuators.find(a.name) )



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::Controller") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::Controller")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Controller, self).writeToStream(writer) :
            return False



        if not self.Cached:

            writer.writeLine("Name \"%s@%s\"" %(self.BlenderController.name, self.Object.name))

            writer.writeLine("Operator %s" %self.Operator)

            writer.writeLine("StateIndex %s" %self.StateIndex)

            writer.moveIn( "ActuatorList %u" % len( self.ActuatorList ) )

            for a in self.ActuatorList:
                writer.writeLine( "%u " %a )

            writer.writeLine( " " )

            writer.moveOut( "ActuatorList %u" % len( self.ActuatorList ) )


        return True
############################################################################




# ........................................................................ #
############################################################################
