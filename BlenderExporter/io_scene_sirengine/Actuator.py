# -*- coding: iso-8859-1 -*-
# file Actuator.py
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
from . import Writable, SoundSource
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Actuator(Writable.Writable):
    """ooGame::Actuator"""





############################################################################
    RigidBody = None
    Object = None
    BlenderActuator = None
    Cached = None
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(Actuator, self).__init__(rigid_body.Data)

        self.RigidBody = rigid_body
        self.Object = rigid_body.Object
        self.BlenderActuator = actuator
        self.Cached = False
############################################################################




############################################################################
    def buildGraph(self):
        super(Actuator, self).buildGraph()

        if self.Data.Cache.has( self.BlenderActuator ):
            self.UniqueID = self.Data.Cache.get( self.BlenderActuator )
            self.Cached = True
        else:
            self.Data.Cache.set( self.BlenderActuator, self.UniqueID )
            self.Cached = False



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::Actuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::Actuator")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Actuator, self).writeToStream(writer) :
            return False



        if not self.Cached:

            writer.writeLine("Name \"%s@%s\"" %(self.BlenderActuator.name, self.Object.name))

            self.writePrivateActuatorData( writer )


        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):

        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class SoundActuator(Actuator):
    """ooGame::SoundActuator"""





############################################################################
    Mode = None
    Sound3D = None
    SoundSource = None
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(SoundActuator, self).__init__(rigid_body, actuator)

        self.Mode = None
        self.Sound3D = None
        self.SoundSource = None
############################################################################




############################################################################
    def buildGraph(self):
        super(SoundActuator, self).buildGraph()

        if self.BlenderActuator.mode == "PLAYSTOP":
            self.Mode = "PLAY_STOP"

        elif self.BlenderActuator.mode == "PLAYEND":
            self.Mode = "PLAY_END"

        elif self.BlenderActuator.mode == "LOOPSTOP":
            self.Mode = "LOOP_STOP"

        elif self.BlenderActuator.mode == "LOOPEND":
            self.Mode = "LOOP_END"



        if self.Data.ExportSounds and self.BlenderActuator.sound and not self.Cached:
            self.SoundSource = SoundSource.SoundSource( self.Data, self.BlenderActuator.sound )
            self.SoundSource.buildGraph()

            self.Sound3D = self.BlenderActuator.use_sound_3d

            if self.BlenderActuator.use_sound_3d:
                self.SoundSource.AutoComputePosition = 'COMPUTE_BY_VIEW'
            else:
                self.SoundSource.AutoComputePosition = 'DO_NOT_AUTOCOMPUTE'

            copy_source = SoundSource.SoundSource( self.Data, self.BlenderActuator.sound, self.SoundSource )
            copy_source.buildGraph()

            self.Data.SoundGroup.addChild( copy_source )




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::SoundActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::SoundActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("Mode %s" % self.Mode)

        if self.Sound3D:
            writer.writeLine("Sound3D TRUE")
        else:
            writer.writeLine("Sound3D FALSE")

        if self.SoundSource:
            writer.moveIn("Source TRUE")
            self.SoundSource.writeToStream( writer )
            writer.moveOut("Source TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class StateActuator(Actuator):
    """ooGame::StateActuator"""





############################################################################
    Operation = None
    Mask = 0
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(StateActuator, self).__init__(rigid_body, actuator)

        self.Operation = "SET"
        self.Mask = 0
############################################################################




############################################################################
    def buildGraph(self):
        super(StateActuator, self).buildGraph()

        self.Operation = self.BlenderActuator.operation


        self.Mask = 0

        for i in range ( len(self.BlenderActuator.states) ):
            self.Mask += int(self.BlenderActuator.states[i]) << i




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::StateActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::StateActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("Operation %s" % self.Operation)
        writer.writeLine("Mask %u" % self.Mask)


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class NodeMaskActuator(Actuator):
    """ooGame::NodeMaskActuator"""





############################################################################
    NodeMask = None
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(NodeMaskActuator, self).__init__(rigid_body, actuator)

        self.NodeMask = 0xFFFFFFFF
############################################################################




############################################################################
    def buildGraph(self):
        super(NodeMaskActuator, self).buildGraph()

        if self.BlenderActuator.use_visible:
            self.NodeMask = 0xFFFFFFFF
        else:
            self.NodeMask = 0x0




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::NodeMaskActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::NodeMaskActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("NodeMask %u" % self.NodeMask)


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class MotionActuator(Actuator):
    """ooGame::MotionActuator"""





############################################################################
    MotionType = None
    Translation = None
    Rotation = None
    Force = None
    Torque = None
    LinearVelocity = None
    AngularVelocity = None
    TranslationLocal = None
    RotationLocal = None
    ForceLocal = None
    TorqueLocal = None
    LinearVelocityLocal = None
    AngularVelocityLocal = None
    LinearVelocityAdditive = None
    ServoLimitX = None
    ServoLimitY = None
    ServoLimitZ = None
    ServoForceMin = None
    ServoForceMax = None

    ServoP = None
    ServoI = None
    ServoD = None
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(MotionActuator, self).__init__(rigid_body, actuator)


        self.MotionType = None
        self.Translation = [0,0,0]
        self.Rotation =  [0,0,0]
        self.Force =  [0,0,0]
        self.Torque =  [0,0,0]
        self.LinearVelocity =  [0,0,0]
        self.AngularVelocity =  [0,0,0]
        self.TranslationLocal = True
        self.RotationLocal = True
        self.ForceLocal = True
        self.TorqueLocal = True
        self.LinearVelocityLocal = False
        self.AngularVelocityLocal = False
        self.LinearVelocityAdditive = False
        self.ServoLimitX = None
        self.ServoLimitY = None
        self.ServoLimitZ = None
        self.ServoForceMin = None
        self.ServoForceMax = None

        self.ServoP = 30.0
        self.ServoI = 0.5
        self.ServoD = 0.0
############################################################################




############################################################################
    def buildGraph(self):
        super(MotionActuator, self).buildGraph()

        if self.BlenderActuator.mode == 'OBJECT_SERVO':
            self.MotionType = "SERVO_CONTROL"
        else:
            self.MotionType = "SIMPLE_MOTION"


        self.Translation = self.BlenderActuator.offset_location
        self.Rotation = self.BlenderActuator.offset_rotation
        self.Force = self.BlenderActuator.force
        self.Torque = self.BlenderActuator.torque
        self.LinearVelocity = self.BlenderActuator.linear_velocity
        self.AngularVelocity = self.BlenderActuator.angular_velocity
        self.TranslationLocal = self.BlenderActuator.use_local_location
        self.RotationLocal = self.BlenderActuator.use_local_rotation
        self.ForceLocal = self.BlenderActuator.use_local_force
        self.TorqueLocal = self.BlenderActuator.use_local_torque
        self.LinearVelocityLocal = self.BlenderActuator.use_local_linear_velocity
        self.AngularVelocityLocal = self.BlenderActuator.use_local_angular_velocity
        self.LinearVelocityAdditive = self.BlenderActuator.use_add_linear_velocity
        self.ServoLimitX = self.BlenderActuator.use_servo_limit_x
        self.ServoLimitY = self.BlenderActuator.use_servo_limit_y
        self.ServoLimitZ = self.BlenderActuator.use_servo_limit_z
        self.ServoForceMin = [ self.BlenderActuator.force_min_x, self.BlenderActuator.force_min_y, self.BlenderActuator.force_min_z ]
        self.ServoForceMax = [ self.BlenderActuator.force_max_x, self.BlenderActuator.force_max_y, self.BlenderActuator.force_max_z ]

        self.ServoP = self.BlenderActuator.proportional_coefficient
        self.ServoI = self.BlenderActuator.integral_coefficient
        self.ServoD = self.BlenderActuator.derivate_coefficient




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::MotionActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::MotionActuator")

        return True
############################################################################




############################################################################
    def writeVec3Prop(self, writer, name, value):
        writer.writeLine("%s %f %f %f" %(name, value[0], value[1], value[2]))
############################################################################




############################################################################
    def writeBoolProp(self, writer, name, value):
        if value:
            writer.writeLine("%s TRUE" %(name))
        else:
            writer.writeLine("%s FALSE" %(name))
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("MotionType %s" % self.MotionType)

        self.writeVec3Prop( writer, "Translation",      self.Translation ) ;
        self.writeVec3Prop( writer, "Rotation",         self.Rotation ) ;
        self.writeVec3Prop( writer, "Force",            self.Force ) ;
        self.writeVec3Prop( writer, "Torque",           self.Torque ) ;
        self.writeVec3Prop( writer, "LinearVelocity",   self.LinearVelocity ) ;
        self.writeVec3Prop( writer, "AngularVelocity",  self.AngularVelocity ) ;
        self.writeVec3Prop( writer, "ServoForceMin",    self.ServoForceMin ) ;
        self.writeVec3Prop( writer, "ServoForceMax",    self.ServoForceMax ) ;

        self.writeBoolProp( writer, "TranslationLocal",         self.TranslationLocal ) ;
        self.writeBoolProp( writer, "RotationLocal",            self.RotationLocal ) ;
        self.writeBoolProp( writer, "ForceLocal",               self.ForceLocal ) ;
        self.writeBoolProp( writer, "TorqueLocal",              self.TorqueLocal ) ;
        self.writeBoolProp( writer, "LinearVelocityLocal",      self.LinearVelocityLocal ) ;
        self.writeBoolProp( writer, "AngularVelocityLocal",     self.AngularVelocityLocal ) ;
        self.writeBoolProp( writer, "LinearVelocityAdditive",   self.LinearVelocityAdditive ) ;
        self.writeBoolProp( writer, "ServoLimitX",              self.ServoLimitX ) ;
        self.writeBoolProp( writer, "ServoLimitY",              self.ServoLimitY ) ;
        self.writeBoolProp( writer, "ServoLimitZ",              self.ServoLimitZ ) ;


        if self.MotionType == "SERVO_CONTROL":

            writer.moveIn("PIDController TRUE")

            writer.moveIn("osgODE::PIDController")
            writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() )
            writer.writeLine( "Name \"PIDController@%s\"" % (self.Object.name) )
            writer.writeLine( "Proportional %f" % (self.ServoP) )
            writer.writeLine( "Integral %f" % (self.ServoI) )
            writer.writeLine( "Derivative %f" % (self.ServoD) )
            writer.moveOut("osgODE::PIDController")

            writer.moveOut("PIDController TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class PropertyActuator(Actuator):
    """ooGame::PropertyActuator"""





############################################################################
    ValueType = None
    Mode = None
    Property = None
    Value = None
    Body = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(PropertyActuator, self).__init__(rigid_body, sensor)

        self.ValueType = None
        self.Mode = None
        self.Property = None
        self.Value = None
        self.ReferenceObject = None
        self.ReferenceProperty = None
############################################################################




############################################################################
    def buildGraph(self):
        super(PropertyActuator, self).buildGraph()

        self.Property = self.BlenderActuator.property

        p = self.RigidBody.getUserValue( self.BlenderActuator.property )

        if not p:
            return False



        self.ValueType = p[2]
        self.Mode = self.BlenderActuator.mode
        self.Property = self.BlenderActuator.property
        self.Value = self.BlenderActuator.value
        self.ReferenceProperty = self.BlenderActuator.object_property

        if self.BlenderActuator.object:
            self.ReferenceObject = self.Data.RigidBodyMap[self.BlenderActuator.object].Name



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::%sPropertyActuator" % self.ValueType) ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::%sPropertyActuator" % self.ValueType)

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("Mode %s" % self.Mode)
        writer.writeLine("Property \"%s\"" % self.Property)

        if self.ReferenceObject:
            writer.writeLine("ReferenceObject \"%s\"" % self.ReferenceObject)

        if self.ReferenceProperty:
            writer.writeLine("ReferenceProperty \"%s\"" % self.ReferenceProperty)



        if self.Value:
            if self.ValueType == "String":
                writer.writeLine("Value \"%s\"" % self.Value)
            else:
                writer.writeLine("Value %s" % self.Value)



        return True
############################################################################




# ........................................................................ #
############################################################################
