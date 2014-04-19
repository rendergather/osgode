# -*- coding: iso-8859-1 -*-
# file Sensor.py
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
from . import Writable, Controller
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Sensor(Writable.Writable):
    """ooGame::Sensor"""





############################################################################
    RigidBody = None
    Object = None
    BlenderSensor = None
    Cached = None
    Invert = None
    Level = None
    Tap = None
    ControllerList = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(Sensor, self).__init__(rigid_body.Data)

        self.RigidBody = rigid_body
        self.Object = rigid_body.Object
        self.BlenderSensor = sensor
        self.Cached = False
        self.Invert = False
        self.Level = False
        self.Tap = False
        self.ControllerList = []
############################################################################




############################################################################
    def buildGraph(self):
        super(Sensor, self).buildGraph()

        if self.Data.Cache.has( self.BlenderSensor ):
            self.UniqueID = self.Data.Cache.get( self.BlenderSensor )
            self.Cached = True
        else:
            self.Data.Cache.set( self.BlenderSensor, self.UniqueID )
            self.Cached = False

            self.Invert = self.BlenderSensor.invert
            self.Level = self.BlenderSensor.use_level
            self.Tap = self.BlenderSensor.use_tap


            for c in self.BlenderSensor.controllers:
                controller = Controller.Controller(self.RigidBody, c)
                controller.buildGraph()
                self.ControllerList.append( controller )



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::Sensor") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::Sensor")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Sensor, self).writeToStream(writer) :
            return False



        if not self.Cached:

            writer.writeLine("Name \"%s@%s\"" %(self.BlenderSensor.name, self.Object.name))



            if self.Invert:
                writer.writeLine("Invert TRUE")
            else:
                writer.writeLine("Invert FALSE")



            if self.Level:
                writer.writeLine("Level TRUE")
            else:
                writer.writeLine("Level FALSE")



            if self.Tap:
                writer.writeLine("Tap TRUE")
            else:
                writer.writeLine("Tap FALSE")



            writer.writeLine( "ControllerList %u" % len( self.ControllerList ) )

            for c in self.ControllerList:
                c.writeToStream( writer )

            self.writePrivateSensorData( writer )


        return True
############################################################################




############################################################################
    def writePrivateSensorData(self, writer):


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class CollisionSensor(Sensor):
    """ooGame::CollisionSensor"""





############################################################################
    RigidBody = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(CollisionSensor, self).__init__(rigid_body, sensor)
############################################################################




############################################################################
    def buildGraph(self):
        super(CollisionSensor, self).buildGraph()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::CollisionSensor") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::CollisionSensor")

        return True
############################################################################




############################################################################
    def writePrivateSensorData(self, writer):

        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class MouseSensor(Sensor):
    """ooGame::MouseSensor"""





############################################################################
    MouseEvent = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(MouseSensor, self).__init__(rigid_body, sensor)

        self.MouseEvent = "LEFT_BUTTON"
############################################################################




############################################################################
    def buildGraph(self):
        super(MouseSensor, self).buildGraph()

        if self.BlenderSensor.mouse_event == "LEFTCLICK":
            self.MouseEvent = "LEFT_BUTTON"

        elif self.BlenderSensor.mouse_event == "MIDDLECLICK":
            self.MouseEvent = "MIDDLE_BUTTON"

        elif self.BlenderSensor.mouse_event == "RIGHTCLICK":
            self.MouseEvent = "RIGHT_BUTTON"

        elif self.BlenderSensor.mouse_event == "MOUSEOVER":
            self.MouseEvent = "MOUSE_OVER"

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::MouseSensor") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::MouseSensor")

        return True
############################################################################




############################################################################
    def writePrivateSensorData(self, writer):


        writer.writeLine("Event %s" % self.MouseEvent)

        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class AlwaysSensor(Sensor):
    """ooGame::AlwaysSensor"""





############################################################################
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(AlwaysSensor, self).__init__(rigid_body, sensor)
############################################################################




############################################################################
    def buildGraph(self):
        super(AlwaysSensor, self).buildGraph()

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::AlwaysSensor") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::AlwaysSensor")

        return True
############################################################################




############################################################################
    def writePrivateSensorData(self, writer):

        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class PropertySensor(Sensor):
    """ooGame::PropertySensor"""





############################################################################
    ValueType = None
    EvaluationType = None
    Property = None
    Value = None
    ValueMin = None
    ValueMax = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(PropertySensor, self).__init__(rigid_body, sensor)

        self.ValueType = None
        self.EvaluationType = None
        self.Property = None
        self.Value = None
        self.ValueMin = None
        self.ValueMax = None
############################################################################




############################################################################
    def buildGraph(self):
        super(PropertySensor, self).buildGraph()

        self.Property = self.BlenderSensor.property

        p = self.RigidBody.getUserValue( self.BlenderSensor.property )

        if not p:
            return False



        self.ValueType = p[2]
        self.EvaluationType = self.BlenderSensor.evaluation_type.replace("PROP", "")
        self.Property = self.BlenderSensor.property
        self.Value = self.BlenderSensor.value
        self.ValueMin = self.BlenderSensor.value_min
        self.ValueMax = self.BlenderSensor.value_max


        if not self.Value:
            self.Value = '0'

        if not self.ValueMin:
            self.ValueMin = '0'

        if not self.ValueMax:
            self.ValueMax = '0'


        if self.ValueType == "Bool":
            if self.Value.lower() in ["1", "true", "on"]:
                self.Value = "TRUE"
            else:
                self.Value = "FALSE"

            if self.ValueMin.lower() in ["1", "true", "on"]:
                self.ValueMin = "TRUE"
            else:
                self.ValueMin = "FALSE"

            if self.ValueMax.lower() in ["1", "true", "on"]:
                self.ValueMax = "TRUE"
            else:
                self.ValueMax = "FALSE"



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::%sPropertySensor" % self.ValueType) ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::%sPropertySensor" % self.ValueType)

        return True
############################################################################




############################################################################
    def writePrivateSensorData(self, writer):


        writer.writeLine("EvaluationType %s" % self.EvaluationType)
        writer.writeLine("Property \"%s\"" % self.Property)



        writer.writeLine("Values")

        indentation = " " * writer.IndentationSize

        if self.ValueType == "String":
            writer.writeLine(indentation + "Value \"%s\"" % self.Value)
            writer.writeLine(indentation + "Min \"%s\"" % self.ValueMin)
            writer.writeLine(indentation + "Max \"%s\"" % self.ValueMax)
        else:
            writer.writeLine(indentation + "Value %s" % self.Value)
            writer.writeLine(indentation + "Min %s" % self.ValueMin)
            writer.writeLine(indentation + "Max %s" % self.ValueMax)



        return True
############################################################################




# ........................................................................ #
############################################################################
