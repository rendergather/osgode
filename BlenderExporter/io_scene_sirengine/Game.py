# -*- coding: iso-8859-1 -*-
# file Game.py
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
from . import ODECallback

import bpy
from . import Sensor, Actuator, Controller
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Game(ODECallback.ODECallback):
    """ooGame::Game"""





############################################################################
    RigidBody = None
    SensorList = None
    ControllerList = None
    ActuatorList = None
    State = None

    Properties = []
############################################################################









############################################################################
    def __init__(self, rigid_body):
        super(Game, self).__init__(rigid_body.Data, rigid_body.Object)

        self.RigidBody = rigid_body
        self.SensorList = []
        self.ControllerList = []
        self.ActuatorList = []
        self.State = 0

        self.Properties = []
############################################################################




############################################################################
    def addProperty(self, pName, pValue, pType):
        self.Properties.append( ( str(pName), str(pValue), str(pType) ) )
############################################################################




############################################################################
    def buildGraph(self):
        super(Game, self).buildGraph()



        self.State = 0

        for i in range ( len(self.Object.game.states_initial) ):
            self.State += int(self.Object.game.states_initial[i]) << i



        for s in self.Object.game.sensors:
            sensor = None

            if s.type == "COLLISION":
                sensor = Sensor.CollisionSensor( self.RigidBody, s )

            elif s.type == "MOUSE":
                sensor = Sensor.MouseSensor( self.RigidBody, s )

            elif s.type == "ALWAYS":
                sensor = Sensor.AlwaysSensor( self.RigidBody, s )

            elif s.type == "PROPERTY":
                sensor = Sensor.PropertySensor( self.RigidBody, s )

            elif s.type == "NEAR":
                sensor = Sensor.NearSensor( self.RigidBody, s )

            elif s.type == "DELAY":
                sensor = Sensor.DelaySensor( self.RigidBody, s )

            elif s.type == "RANDOM":
                sensor = Sensor.RandomSensor( self.RigidBody, s )

            elif s.type == "KEYBOARD":
                sensor = Sensor.KeyboardSensor( self.RigidBody, s )

            elif s.type == "MESSAGE":
                sensor = Sensor.MessageSensor( self.RigidBody, s )

            if sensor:
                sensor.buildGraph()
                self.SensorList.append( sensor )
            else:
                self.Data.Operator.report({'ERROR'}, "[%s] Unsupported sensor type: %s" %(self.Object.name, s.type))
                return False



        for c in self.Object.game.controllers:
            controller = Controller.Controller(self.RigidBody, c)
            controller.buildGraph()
            self.ControllerList.append( controller )



        for a in self.Object.game.actuators:

            actuator = None

            if a.type == "SOUND":
                actuator = Actuator.SoundActuator(self.RigidBody, a)

            elif a.type == "STATE":
                actuator = Actuator.StateActuator(self.RigidBody, a)

            elif a.type == "VISIBILITY":
                actuator = Actuator.NodeMaskActuator(self.RigidBody, a)

            elif a.type == "MOTION":
                actuator = Actuator.MotionActuator(self.RigidBody, a)

            elif a.type == "PROPERTY":
                actuator = Actuator.PropertyActuator(self.RigidBody, a)

            elif a.type == "SCENE":
                actuator = Actuator.SceneActuator(self.RigidBody, a)

            elif a.type == "EDIT_OBJECT":
                actuator = Actuator.EditObjectActuator(self.RigidBody, a)

            elif a.type == "CAMERA":
                actuator = Actuator.CameraActuator(self.RigidBody, a)

            elif a.type == "MESSAGE":
                actuator = Actuator.MessageActuator(self.RigidBody, a)

            elif a.type == "GAME":
                actuator = Actuator.GameActuator(self.RigidBody, a)

            elif a.type == "MOUSE":
                actuator = Actuator.MouseActuator(self.RigidBody, a)


            if actuator:
                actuator.buildGraph()
                self.ActuatorList.append( actuator )
            else:
                self.Data.Operator.report({'ERROR'}, "[%s] Unsupported actuator type: %s" %(self.Object.name, a.type))
                return False




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::Game") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::Game")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Game, self).writePrivateData(writer) :
            return False




        if self.Properties != [] :
            writer.moveIn( "UserDataContainer TRUE" )
            writer.moveIn( "osg::DefaultUserDataContainer" )



            writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() )

            writer.moveIn( "UDC_UserObjects %d" %len(self.Properties) )


            for i in self.Properties:
                writer.moveIn( "osg::%sValueObject" %i[2] ) ;

                writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() ) ;

                writer.writeLine( "Name \"%s\"" % str(i[0]) ) ;


                if i[2] == "String":
                    writer.writeLine( "Value \"%s\"" % str(i[1]) ) ;
                else:
                    writer.writeLine( "Value %s" % str(i[1]) ) ;

                writer.moveOut( "osg::%sValueObject" %i[2] ) ;


            writer.moveOut( "UDC_UserObjects %d" %len(self.Properties) )



            writer.moveOut( "osg::DefaultUserDataContainer" )
            writer.moveOut( "UserDataContainer TRUE" )




        writer.writeLine( "State %u" % self.State )




        writer.writeLine( "SensorList %u" % len(self.SensorList) )

        for s in self.SensorList:
            s.writeToStream(writer)




        writer.writeLine( "ControllerList %u" % len(self.ControllerList) )

        for c in self.ControllerList:
            c.writeToStream(writer)




        writer.writeLine( "ActuatorList %u" % len(self.ActuatorList) )

        for a in self.ActuatorList:
            a.writeToStream(writer)


        return True
############################################################################




# ........................................................................ #
############################################################################
