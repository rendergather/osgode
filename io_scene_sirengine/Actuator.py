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
    Gain = None
    Pitch = None

    UseGainLevels = None
    GainLevels = None
############################################################################









############################################################################
    def __init__(self, rigid_body, actuator):
        super(SoundActuator, self).__init__(rigid_body, actuator)

        self.Mode = None
        self.Sound3D = None
        self.SoundSource = None

        self.Gain = 1.0
        self.Pitch = 1.0

        self.UseGainLevels = False
        self.GainLevels = [0.5, 3.0, 10.0]
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


        self.Gain = self.BlenderActuator.volume
        self.Pitch = self.BlenderActuator.pitch / 12.0 + 1.0



        if self.Data.ExportSounds and self.BlenderActuator.sound and not self.Cached:
            self.SoundSource = SoundSource.SoundSource( self.Data, self.BlenderActuator.sound )
            self.SoundSource.buildGraph()

            self.Sound3D = self.BlenderActuator.use_sound_3d

            if self.BlenderActuator.use_sound_3d:
                self.SoundSource.AutoComputePosition = 'COMPUTE_BY_VIEW'
            else:
                self.SoundSource.AutoComputePosition = 'DO_NOT_AUTOCOMPUTE'


            self.SoundSource.MAX_DISTANCE = self.BlenderActuator.distance_3d_max
            self.SoundSource.ROLLOFF_FACTOR = self.BlenderActuator.rolloff_factor_3d
            self.SoundSource.REFERENCE_DISTANCE = self.BlenderActuator.distance_3d_reference
            self.SoundSource.MIN_GAIN = self.BlenderActuator.gain_3d_min
            self.SoundSource.MAX_GAIN = self.BlenderActuator.gain_3d_max
            self.SoundSource.CONE_OUTER_GAIN = self.BlenderActuator.cone_outer_gain_3d
            self.SoundSource.CONE_INNER_ANGLE = self.BlenderActuator.cone_inner_angle_3d
            self.SoundSource.CONE_OUTER_ANGLE = self.BlenderActuator.cone_outer_angle_3d



        try:
            gain_levels = []
            gain_levels.append( self.Object["oog_%s_gain_min" % self.BlenderActuator.sound.name] )
            gain_levels.append( self.Object["oog_%s_gain_mid" % self.BlenderActuator.sound.name] )
            gain_levels.append( self.Object["oog_%s_gain_max" % self.BlenderActuator.sound.name] )

            self.UseGainLevels = True
            self.GainLevels = gain_levels
        except:
            self.UseGainLevels = False




        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        if self.UseGainLevels:
            writer.moveIn("ooGame::CollisionSoundActuator")
        else:
            writer.moveIn("ooGame::SoundActuator")

        self.writePrivateData(writer)

        if self.UseGainLevels:
            writer.moveOut("ooGame::CollisionSoundActuator")
        else:
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


        writer.writeLine("Gain %f" % self.Gain)
        writer.writeLine("Pitch %f" % self.Pitch)



        if self.UseGainLevels:
            writer.writeLine("UseGainLevels TRUE")
            writer.writeLine("GainLevels %f %f %f" % ( float(self.GainLevels[0]), float(self.GainLevels[1]), float(self.GainLevels[2]) ))



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

        if self.ValueType == "Bool":
            if self.Value.lower().split() in [["1"], ["true"], ["on"]]:
                self.Value = "TRUE"
            else:
                self.Value = "FALSE"

        if self.BlenderActuator.object:
            try:
                self.ReferenceObject = self.Data.RigidBodyMap[self.BlenderActuator.object].Name
            except:
                print("OOE PropertyActuator: cannot find ReferenceObject: " + self.BlenderActuator.name + "@" + self.Object.name + " : " + self.BlenderActuator.object.name)
                return False



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









############################################################################
# ........................................................................ #
class SceneActuator(Actuator):
    """ooGame::SceneActuator"""





############################################################################
    Mode = None
    Scene = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(SceneActuator, self).__init__(rigid_body, sensor)

        self.Mode = None
        self.Scene = None
############################################################################




############################################################################
    def buildGraph(self):
        super(SceneActuator, self).buildGraph()

        self.Mode = self.BlenderActuator.mode

        try:
            self.Scene = self.BlenderActuator.scene.name
        except:
            self.Scene = None



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::SceneActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::SceneActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("Mode %s" % self.Mode)

        if self.Scene:
            writer.writeLine("SceneName \"%s\"" % self.Scene)


        if self.Mode == 'CAMERA':
            writer.writeLine( "RemoveExistingManipulator TRUE" )



        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class EditObjectActuator(Actuator):
    """ooGame::EditObjectActuator"""





############################################################################
    Mode = None
    DynamicOperation = None
    LifeTime = None
    LinearVelocity = None
    AngularVelocity = None
    LinearVelocityLocal = None
    AngularVelocityLocal = None
    Mass = None
    RefObject = None
    TrackToByName = None
    AngularPID = [1,0,0]
    LinearPID = [1,0,0]
    DistanceMin = None
    DistanceMax = None
    UpWorld = None
    UpLocal = None
    FrontLocal = None
    Mesh = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(EditObjectActuator, self).__init__(rigid_body, sensor)

        self.Mode = None
        self.DynamicOperation = None
        self.LifeTime = None
        self.LinearVelocity = None
        self.AngularVelocity = None
        self.LinearVelocityLocal = None
        self.AngularVelocityLocal = None
        self.Mass = None
        self.RefObject = None
        self.TrackToByName = None
        self.AngularPID = [1,0,0]
        self.LinearPID = [1,0,0]
        self.DistanceMin = None
        self.DistanceMax = None
        self.UpWorld = [0,0,1]
        self.UpLocal = [0,0,1]
        self.FrontLocal = [0,1,0]
        self.Mesh = None
############################################################################




############################################################################
    def buildGraph(self):
        super(EditObjectActuator, self).buildGraph()

        if self.BlenderActuator.mode == "ADDOBJECT":
            self.Mode = "ADD_OBJECT"

        elif self.BlenderActuator.mode == "ENDOBJECT":
            self.Mode = "END_OBJECT"

        elif self.BlenderActuator.mode == "TRACKTO":
            self.Mode = "TRACK_TO"

        elif self.BlenderActuator.mode == "DYNAMICS":
            self.Mode = "DYNAMICS"

        elif self.BlenderActuator.mode == "REPLACEMESH":
            self.Mode = "REPLACE_MESH"

        else:
            return False


        if self.Mode == "DYNAMICS":
            if self.BlenderActuator.dynamic_operation == "SETMASS":
                self.DynamicOperation = "SET_MASS"

            elif self.BlenderActuator.dynamic_operation == "ENABLERIGIDBODY":
                self.DynamicOperation = "ENABLE_RIGID_BODY"

            elif self.BlenderActuator.dynamic_operation == "DISABLERIGIDBODY":
                self.DynamicOperation = "DISABLE_RIGID_BODY"

            elif self.BlenderActuator.dynamic_operation == "SUSPENDDYN":
                self.DynamicOperation = "SET_KINEMATIC"

            elif self.BlenderActuator.dynamic_operation == "RESTOREDYN":
                self.DynamicOperation = "SET_DYNAMIC"

            else:
                return False



        self.LifeTime = self.BlenderActuator.time

        self.LinearVelocity = self.BlenderActuator.linear_velocity
        self.AngularVelocity = self.BlenderActuator.angular_velocity

        if self.BlenderActuator.use_local_linear_velocity:
            self.LinearVelocityLocal = "TRUE"
        else:
            self.LinearVelocityLocal = "FALSE"

        if self.BlenderActuator.use_local_angular_velocity:
            self.AngularVelocityLocal = "TRUE"
        else:
            self.AngularVelocityLocal = "FALSE"



        self.Mass = self.BlenderActuator.mass


        obj = None


        if self.Mode == "TRACK_TO":
            self.TrackToByName = self.Data.RigidBodyMap[self.BlenderActuator.object].Name


        elif self.Mode == "REPLACE_MESH":

            from . import ODETransform
            try:
                self.Mesh = ODETransform.ODETransform(self.Data, self.Data.Scene.objects[self.BlenderActuator.mesh.name])
            except:
                print("OOE failed REPLACE_MESH: " + self.BlenderActuator.name + "@" + self.Object.name)
                return False


        elif self.Mode == "ADD_OBJECT":

            obj = self.BlenderActuator.object

            if obj:

                if not obj.game.use_collision_bounds:

                    from . import RigidBody
                    self.RefObject = RigidBody.RigidBody(self.Data, obj)



                elif obj.game.physics_type == 'NO_COLLISION' :

                    from . import RigidBody
                    self.RefObject = RigidBody.RigidBody(self.Data, obj)





                elif obj.game.physics_type in ['STATIC', 'DYNAMIC', 'RIGID_BODY']:
                    if obj.game.collision_bounds_type == 'TRIANGLE_MESH':
                        from . import TriMesh
                        self.RefObject = TriMesh.TriMesh(self.Data, obj)

                    elif obj.game.collision_bounds_type == 'BOX':
                        from . import Box
                        self.RefObject = Box.Box(self.Data, obj)

                    elif obj.game.collision_bounds_type == 'SPHERE':
                        from . import Sphere
                        self.RefObject = Sphere.Sphere(self.Data, obj)

                    elif obj.game.collision_bounds_type == 'CYLINDER':
                        from . import Cylinder
                        self.RefObject = Cylinder.Cylinder(self.Data, obj)

                    elif obj.game.collision_bounds_type == 'CAPSULE':
                        from . import Capsule
                        self.RefObject = Capsule.Capsule(self.Data, obj)

                    else:
                        print("This exporter does not support %s collision bounds" %obj.game.collision_bounds_type)
                        return False




                else:
                    print("EditObjectActuator does not support %s physics type" %obj.game.physics_type)
                    return False



        if self.Mesh:
            self.Mesh.buildGraph()



        if self.RefObject:
            self.RefObject.buildGraph()


            if ( len(self.RefObject.Object.game.sensors) + len(self.RefObject.Object.game.properties) ) > 0:

                from . import Game

                game = Game.Game( self.RefObject )
                game.buildGraph()

                self.RefObject.addUpdateCallback( game )



        try:
            self.AngularPID[0] = self.Object["oog_%s_AP" % self.BlenderActuator.name]
        except:
            self.AngularPID[0] = 1.0

        try:
            self.AngularPID[1] = self.Object["oog_%s_AI" % self.BlenderActuator.name]
        except:
            self.AngularPID[1] = 0.0

        try:
            self.AngularPID[2] = self.Object["oog_%s_AD" % self.BlenderActuator.name]
        except:
            self.AngularPID[2] = 0.0



        try:
            self.LinearPID[0] = self.Object["oog_%s_LP" % self.BlenderActuator.name]
        except:
            self.LinearPID[0] = 1.0

        try:
            self.LinearPID[1] = self.Object["oog_%s_LI" % self.BlenderActuator.name]
        except:
            self.LinearPID[1] = 0.0

        try:
            self.LinearPID[2] = self.Object["oog_%s_LD" % self.BlenderActuator.name]
        except:
            self.LinearPID[2] = 0.0




        if self.BlenderActuator.track_axis == "TRACKAXISX":
            self.FrontLocal = [1,0,0]

        elif self.BlenderActuator.track_axis == "TRACKAXISY":
            self.FrontLocal = [0,1,0]

        elif self.BlenderActuator.track_axis == "TRACKAXISZ":
            self.FrontLocal = [0,0,1]

        elif self.BlenderActuator.track_axis == "TRACKAXISNEGX":
            self.FrontLocal = [-1,0,0]

        elif self.BlenderActuator.track_axis == "TRACKAXISNEGY":
            self.FrontLocal = [0,-1,0]

        elif self.BlenderActuator.track_axis == "TRACKAXISNEGZ":
            self.FrontLocal = [0,0,-1]




        if self.BlenderActuator.up_axis == "UPAXISX":
            self.UpWorld = [1,0,0]

        elif self.BlenderActuator.up_axis == "UPAXISY":
            self.UpWorld = [0,1,0]

        elif self.BlenderActuator.up_axis == "UPAXISZ":
            self.UpWorld = [0,0,1]



        try:
            up_local = self.Object["oog_%s_UpLocal" % self.BlenderActuator.name]

            if up_local == 0:
                self.UpLocal = [1,0,0]
            elif up_local == 1:
                self.UpLocal = [0,1,0]
            elif up_local == 2:
                self.UpLocal = [0,0,1]
        except:
                self.UpLocal = None



        try:
            self.DistanceMin = self.Object["oog_%s_DistanceMin" % self.BlenderActuator.name]
        except:
            self.DistanceMin = None

        try:
            self.DistanceMax = self.Object["oog_%s_DistanceMax" % self.BlenderActuator.name]
        except:
            self.DistanceMax = None


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::EditObjectActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::EditObjectActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        writer.writeLine("Mode %s" % self.Mode)

        if self.DynamicOperation:
            writer.writeLine("DynamicOperation %s" % self.DynamicOperation)

        if self.LifeTime:
            writer.writeLine("LifeTime %u" % self.LifeTime)

        if self.LinearVelocity:
            writer.writeLine("LinearVelocity %f %f %f" %(self.LinearVelocity[0], self.LinearVelocity[1], self.LinearVelocity[2]))

        if self.AngularVelocity:
            writer.writeLine("AngularVelocity %f %f %f" %(self.AngularVelocity[0], self.AngularVelocity[1], self.AngularVelocity[2]))

        if self.LinearVelocity:
            writer.writeLine("LinearVelocityLocal %s" %(self.LinearVelocityLocal))


        if self.AngularVelocity:
            writer.writeLine("AngularVelocityLocal %s" %(self.AngularVelocityLocal))


        if self.Mass:
            writer.writeLine("Mass %f" % self.Mass)


        if self.RefObject:

            writer.moveIn("Object TRUE")
            self.RefObject.writeToStream( writer )
            writer.moveOut("Object TRUE")



        if self.UpWorld:
            writer.writeLine( "UpWorld %f %f %f" % (self.UpWorld[0], self.UpWorld[1], self.UpWorld[2]) )

        if self.UpLocal:
            writer.writeLine( "UpLocal %f %f %f" % (self.UpLocal[0], self.UpLocal[1], self.UpLocal[2]) )

        if self.FrontLocal:
            writer.writeLine( "FrontLocal %f %f %f" % (self.FrontLocal[0], self.FrontLocal[1], self.FrontLocal[2]) )




        if self.DistanceMin != None:
            writer.writeLine( "DistanceMin %f" % self.DistanceMin )

        if self.DistanceMax != None:
            writer.writeLine( "DistanceMax %f" % self.DistanceMax )



        if self.TrackToByName:
            writer.writeLine("TrackToByNameInternal \"%s\"" % self.TrackToByName)



        if self.TrackToByName:


            writer.moveIn("LinearPID TRUE")
            writer.moveIn("osgODE::PIDController")

            writer.writeLine("UniqueID %u" % self.Data.UniqueID.generate())
            writer.writeLine("Name \"LinearPIDController@%s\"" % (self.Object.name) )
            writer.writeLine("Proportional %f" % self.LinearPID[0])
            writer.writeLine("Integral %f" % self.LinearPID[1])
            writer.writeLine("Derivative %f" % self.LinearPID[2])

            writer.moveOut("osgODE::PIDController")
            writer.moveOut("LinearPID TRUE")



            writer.moveIn("AngularPID TRUE")
            writer.moveIn("osgODE::PIDController")

            writer.writeLine("UniqueID %u" % self.Data.UniqueID.generate())
            writer.writeLine("Name \"AngularPIDController@%s\"" % (self.Object.name) )
            writer.writeLine("Proportional %f" % self.AngularPID[0])
            writer.writeLine("Integral %f" % self.AngularPID[1])
            writer.writeLine("Derivative %f" % self.AngularPID[2])

            writer.moveOut("osgODE::PIDController")
            writer.moveOut("AngularPID TRUE")



        if self.Mesh:
            writer.moveIn("Mesh TRUE")

            if not self.Mesh.writeToStream(writer):
                return False

            writer.moveOut("Mesh TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class CameraActuator(Actuator):
    """ooGame::CameraActuator"""





############################################################################
    TrackToByName = None
    Height = None
    DistanceMin = None
    DistanceMax = None
    UpWorld = None
    Axis = None
    LinearPID = None
    AngularPID = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(CameraActuator, self).__init__(rigid_body, sensor)

        self.TrackToByName = None
        self.Height = 0.0
        self.DistanceMin = 0.0
        self.DistanceMax = 0.0
        self.UpWorld = [0,0,1]
        self.Axis = [1,0,0]
        self.AngularPID = [1,0,0]
        self.LinearPID = [1,0,0]
############################################################################




############################################################################
    def buildGraph(self):
        super(CameraActuator, self).buildGraph()


        self.TrackToByName = self.Data.RigidBodyMap[self.BlenderActuator.object].Name
        self.Height = self.BlenderActuator.height
        self.DistanceMin = self.BlenderActuator.min
        self.DistanceMax = self.BlenderActuator.max
        self.Height = self.BlenderActuator.height



        if self.BlenderActuator.axis == "POS_X" :
            self.Axis = [1, 0, 0]

        if self.BlenderActuator.axis == "NEG_X" :
            self.Axis = [-1, 0, 0]

        if self.BlenderActuator.axis == "POS_Y" :
            self.Axis = [0, 1, 0]

        if self.BlenderActuator.axis == "NEG_Y" :
            self.Axis = [0, -1, 0]


        self.LinearPID[0] = self.BlenderActuator.damping
        self.AngularPID[0] = self.BlenderActuator.damping



        try:
            #self.AngularPID[0] = self.Object["oog_%s_AP" % self.BlenderActuator.name]
            self.AngularPID[0] = self.Object["oog_camera_AP"]
        except:
            self.AngularPID[0] = 1.0

        try:
            #self.AngularPID[1] = self.Object["oog_%s_AI" % self.BlenderActuator.name]
            self.AngularPID[1] = self.Object["oog_camera_AI"]
        except:
            self.AngularPID[1] = 0.0

        try:
            #self.AngularPID[2] = self.Object["oog_%s_AD" % self.BlenderActuator.name]
            self.AngularPID[2] = self.Object["oog_camera_AD"]
        except:
            self.AngularPID[2] = 0.0



        try:
            #self.LinearPID[0] = self.Object["oog_%s_LP" % self.BlenderActuator.name]
            self.LinearPID[0] = self.Object["oog_camera_LP"]
        except:
            self.LinearPID[0] = 1.0

        try:
            #self.LinearPID[1] = self.Object["oog_%s_LI" % self.BlenderActuator.name]
            self.LinearPID[1] = self.Object["oog_camera_LI"]
        except:
            self.LinearPID[1] = 0.0

        try:
            #self.LinearPID[2] = self.Object["oog_%s_LD" % self.BlenderActuator.name]
            self.LinearPID[2] = self.Object["oog_camera_LD"]
        except:
            self.LinearPID[2] = 0.0



        try:
            upw = self.Object["oog_%s_up_world" % self.BlenderActuator.name]

            if upw == 0:
                self.UpWorld = [1,0,0]
            if upw == 1:
                self.UpWorld = [0,1,0]
            if upw == 2:
                self.UpWorld = [0,0,1]

            if upw == 3:
                self.UpWorld = [-1,0,0]
            if upw == 4:
                self.UpWorld = [0,-1,0]
            if upw == 5:
                self.UpWorld = [0,0,-1]
        except:
            self.UpWorld = None


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::CameraActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::CameraActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):


        if self.Axis:
            writer.writeLine( "Axis %f %f %f" % (self.Axis[0], self.Axis[1], self.Axis[2]) )


        if self.UpWorld:
            writer.writeLine( "UpWorld %f %f %f" % (self.UpWorld[0], self.UpWorld[1], self.UpWorld[2]) )


        if self.DistanceMin:
            writer.writeLine( "DistanceMin %f" % self.DistanceMin )


        if self.DistanceMax:
            writer.writeLine( "DistanceMax %f" % self.DistanceMax )


        if self.Height != None:
            writer.writeLine( "Height %f" % self.Height )



        if self.TrackToByName:
            writer.writeLine("TrackToByNameInternal \"%s\"" % self.TrackToByName)



        if self.TrackToByName:


            writer.moveIn("LinearPID TRUE")
            writer.moveIn("osgODE::PIDController")

            writer.writeLine("UniqueID %u" % self.Data.UniqueID.generate())
            writer.writeLine("Name \"LinearPIDController@%s\"" % (self.Object.name) )
            writer.writeLine("Proportional %f" % self.LinearPID[0])
            writer.writeLine("Integral %f" % self.LinearPID[1])
            writer.writeLine("Derivative %f" % self.LinearPID[2])

            writer.moveOut("osgODE::PIDController")
            writer.moveOut("LinearPID TRUE")



            writer.moveIn("AngularPID TRUE")
            writer.moveIn("osgODE::PIDController")

            writer.writeLine("UniqueID %u" % self.Data.UniqueID.generate())
            writer.writeLine("Name \"AngularPIDController@%s\"" % (self.Object.name) )
            writer.writeLine("Proportional %f" % self.AngularPID[0])
            writer.writeLine("Integral %f" % self.AngularPID[1])
            writer.writeLine("Derivative %f" % self.AngularPID[2])

            writer.moveOut("osgODE::PIDController")
            writer.moveOut("AngularPID TRUE")



        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class MessageActuator(Actuator):
    """ooGame::MessageActuator"""





############################################################################
    TargetByName = None
    Type = None
    Subject = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(MessageActuator, self).__init__(rigid_body, sensor)

        self.TargetByName = None
        self.Type = "TEXT"
        self.Subject = None
############################################################################




############################################################################
    def buildGraph(self):
        super(MessageActuator, self).buildGraph()

        try:
            self.TargetByName = self.Data.RigidBodyNames[self.BlenderActuator.to_property]
        except:
            print("OOE Missing to_property: " + self.BlenderActuator.name + "@" + self.Object.name)
            self.TargetByName = ""

        self.Type = "TEXT"
        self.Subject = self.BlenderActuator.subject


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::MessageActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::MessageActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):

        if self.Type:
            writer.writeLine("Type %s" % self.Type)

        if self.Subject:
            writer.writeLine("Subject \"%s\"" % self.Subject)


        if self.TargetByName:
            writer.writeLine("TargetByNameInternal \"%s\"" % self.TargetByName)



        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class GameActuator(Actuator):
    """ooGame::GameActuator"""





############################################################################
    FileName = None
    Mode = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(GameActuator, self).__init__(rigid_body, sensor)

        self.FileName = None
        self.Mode = None
############################################################################




############################################################################
    def buildGraph(self):
        super(GameActuator, self).buildGraph()

        self.FileName = self.BlenderActuator.filename


        if self.BlenderActuator.mode == "QUIT" :
            self.Mode = "QUIT"

        elif self.BlenderActuator.mode == "RESTART" :
            self.Mode = "RESTART"

        elif self.BlenderActuator.mode == "START" :
            self.Mode = "START_FROM_FILE"

        else:
            self.Mode = "USER"


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::GameActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::GameActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):

        if self.Mode:
            writer.writeLine("Mode %s" % self.Mode)


        if self.FileName:
            writer.writeLine("FileName \"%s\"" % self.FileName)



        return True
############################################################################




# ........................................................................ #
############################################################################









############################################################################
# ........................................................................ #
class MouseActuator(Actuator):
    """ooGame::MouseActuator"""





############################################################################
    Mode = None

    UseAxisX = None
    SensitivityX = None
    ThresholdX = None
    MinX = None
    MaxX = None
    ObjectAxisX = None
    LocalX = None
    ResetX = None

    UseAxisY = None
    SensitivityY = None
    ThresholdY = None
    MinY = None
    MaxY = None
    ObjectAxisY = None
    LocalY = None
    ResetY = None
############################################################################









############################################################################
    def __init__(self, rigid_body, sensor):
        super(MouseActuator, self).__init__(rigid_body, sensor)

        self.Mode = None

        self.UseAxisX = None
        self.SensitivityX = None
        self.ThresholdX = None
        self.MinX = None
        self.MaxX = None
        self.ObjectAxisX = None
        self.LocalX = None
        self.ResetX = None

        self.UseAxisY = None
        self.SensitivityY = None
        self.ThresholdY = None
        self.MinY = None
        self.MaxY = None
        self.ObjectAxisY = None
        self.LocalY = None
        self.ResetY = None
############################################################################




############################################################################
    def buildGraph(self):
        super(MouseActuator, self).buildGraph()

        self.Mode = self.BlenderActuator.mode



        try:
            if self.Object["oog_%s_MOVE" % self.BlenderActuator.name] != 0:
                self.Mode = "MOVE"
        except:
            self.Mode = self.BlenderActuator.mode



        self.SensitivityX = self.BlenderActuator.sensitivity_x
        self.ThresholdX = self.BlenderActuator.threshold_x
        self.MinX = self.BlenderActuator.min_x
        self.MaxX = self.BlenderActuator.max_x

        if self.BlenderActuator.use_axis_x:
            self.UseAxisX = "TRUE"
        else:
            self.UseAxisX = "FALSE"

        if self.BlenderActuator.local_x:
            self.LocalX = "TRUE"
        else:
            self.LocalX = "FALSE"

        if self.BlenderActuator.reset_x:
            self.ResetX = "TRUE"
        else:
            self.ResetX = "FALSE"

        if self.BlenderActuator.object_axis_x == "OBJECT_AXIS_X":
            self.ObjectAxisX = "1 0 0"
        elif self.BlenderActuator.object_axis_x == "OBJECT_AXIS_Y":
            self.ObjectAxisX = "0 1 0"
        elif self.BlenderActuator.object_axis_x == "OBJECT_AXIS_Z":
            self.ObjectAxisX = "0 0 1"



        self.SensitivityY = self.BlenderActuator.sensitivity_y
        self.ThresholdY = self.BlenderActuator.threshold_y
        self.MinY = self.BlenderActuator.min_y
        self.MaxY = self.BlenderActuator.max_y

        if self.BlenderActuator.use_axis_y:
            self.UseAxisY = "TRUE"
        else:
            self.UseAxisY = "FALSE"

        if self.BlenderActuator.local_y:
            self.LocalY = "TRUE"
        else:
            self.LocalY = "FALSE"

        if self.BlenderActuator.reset_y:
            self.ResetY = "TRUE"
        else:
            self.ResetY = "FALSE"

        if self.BlenderActuator.object_axis_y == "OBJECT_AXIS_X":
            self.ObjectAxisY = "1 0 0"
        elif self.BlenderActuator.object_axis_y == "OBJECT_AXIS_Y":
            self.ObjectAxisY = "0 1 0"
        elif self.BlenderActuator.object_axis_y == "OBJECT_AXIS_Z":
            self.ObjectAxisY = "0 0 1"


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("ooGame::MouseActuator") ;

        self.writePrivateData(writer)

        writer.moveOut("ooGame::MouseActuator")

        return True
############################################################################




############################################################################
    def writePrivateActuatorData(self, writer):

        writer.writeLine("Mode %s" % self.Mode)


        writer.writeLine("UseAxisX %s" %self.UseAxisX)
        writer.writeLine("SensitivityX %f" %self.SensitivityX)
        writer.writeLine("ThresholdX %f" %self.ThresholdX)

        if self.MinX:
            writer.writeLine("MinX %f" %self.MinX)

        if self.MaxX:
            writer.writeLine("MaxX %f" %self.MaxX)

        writer.writeLine("ObjectAxisX %s" %self.ObjectAxisX)
        writer.writeLine("LocalX %s" %self.LocalX)
        writer.writeLine("ResetX %s" %self.ResetX)


        writer.writeLine("UseAxisY %s" %self.UseAxisY)
        writer.writeLine("SensitivityY %f" %self.SensitivityY)
        writer.writeLine("ThresholdY %f" %self.ThresholdY)

        if self.MinY:
            writer.writeLine("MinY %f" %self.MinY)

        if self.MaxY:
            writer.writeLine("MaxY %f" %self.MaxY)

        writer.writeLine("ObjectAxisY %s" %self.ObjectAxisY)
        writer.writeLine("LocalY %s" %self.LocalY)
        writer.writeLine("ResetY %s" %self.ResetY)



        return True
############################################################################




# ........................................................................ #
############################################################################
