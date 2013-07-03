# -*- coding: iso-8859-1 -*-
# file Joint.py
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
from . import ODEObject

from mathutils import Vector, Matrix, Euler
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Joint(ODEObject.ODEObject):
    """Joint in the scene"""





############################################################################
    Constraint = None
    JointType = None
    Body1 = None
    Body2 = None
    Anchor1 = None
    Anchor2 = None
    Axis1 = None
    Axis2 = None
    Axis3 = None
    LoStop = None
    HiStop = None
    LoStop2 = None
    HiStop2 = None
    LoStop3 = None
    HiStop3 = None
    Axis1Anchor = None
    Axis2Anchor = None
    Axis3Anchor = None
    MotorMode = None
    NumAxes = None
    TransformUniqueID = None
############################################################################









############################################################################
    def __init__(self, data, constraint, obj):
        super(Joint, self).__init__(data, constraint)

        self.Constraint = obj
        self.JointType = None
        self.Body1 = None
        self.Body2 = None
        self.Anchor1 = None
        self.Anchor2 = None
        self.Axis1 = None
        self.Axis2 = None
        self.Axis3 = None
        self.LoStop = None
        self.HiStop = None
        self.LoStop2 = None
        self.HiStop2 = None
        self.LoStop3 = None
        self.HiStop3 = None
        self.Axis1Anchor = None
        self.Axis2Anchor = None
        self.Axis3Anchor = None
        self.MotorMode = None
        self.NumAxes = None
        self.TransformUniqueID = None
############################################################################




############################################################################
    def buildGraph(self):
        super(Joint, self).buildGraph()


        axis_order = (0, 1, 2)


        if self.Object.pivot_type == "BALL":
            self.JointType = "osgODE::BallJoint"
        elif self.Object.pivot_type == "HINGE":
            self.JointType = "osgODE::HingeJoint"

        elif self.Object.pivot_type == "GENERIC_6_DOF":
            self.JointType, \
            axis_order = self.selectJointTypeBy6DOF(self.Object)

        else:
            self.Data.Operator.report({'ERROR'}, "Unsupported constraint type (%s)" %self.Object.pivot_type)
            return False


        if not self.JointType:
            self.Data.Operator.report({'ERROR'}, "Unsupported constraint type (%s)" %self.JointType)
            return False


        self.Body1 = self.Constraint
        self.Body2 = self.Object.target


        v = Vector()
        v.x = self.Object.pivot_x
        v.y = self.Object.pivot_y
        v.z = self.Object.pivot_z

        v = self.Constraint.matrix_world * v

        self.Anchor1 = v
        self.Anchor2 = v



        rx = self.Object.axis_x
        ry = self.Object.axis_y
        rz = self.Object.axis_z

        m1 = Matrix().to_3x3()
        m2 = Matrix().to_3x3()
        m3 = Matrix().to_3x3()

        m1.rotate( Euler( (-rx, 0.0, 0.0) ) )
        m2.rotate( Euler( (0.0, -ry, 0.0) ) )
        m3.rotate( Euler( (0.0, 0.0, -rz) ) )

        m = m1 * m2 * m3


        rot_matrix = Matrix(self.Constraint.matrix_world).to_3x3()

        self.Axis1 = Vector()
        self.Axis1.x = m[ axis_order[0] ][0]
        self.Axis1.y = m[ axis_order[0] ][1]
        self.Axis1.z = m[ axis_order[0] ][2]
        self.Axis1 = rot_matrix * self.Axis1

        self.Axis2 = Vector()
        self.Axis2.x = m[ axis_order[1] ][0]
        self.Axis2.y = m[ axis_order[1] ][1]
        self.Axis2.z = m[ axis_order[1] ][2]
        self.Axis2 = rot_matrix * self.Axis2

        self.Axis3 = Vector()
        self.Axis3.x = m[ axis_order[2] ][0]
        self.Axis3.y = m[ axis_order[2] ][1]
        self.Axis3.z = m[ axis_order[2] ][2]
        self.Axis3 = rot_matrix * self.Axis3

        self.Axis1.normalize()
        self.Axis2.normalize()
        self.Axis3.normalize()


        if self.JointType == "osgODE::HingeJoint" and self.Object.use_angular_limit_x:
            self.LoStop = self.Object.limit_angle_min_x
            self.HiStop = self.Object.limit_angle_max_x



        self.TransformUniqueID = self.Data.UniqueID.generate()


        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        if not (self.Body1 or self.Body2):
            return False

        writer.moveIn(self.JointType)

        if not self.writePrivateData(writer):
            return False

        writer.moveOut(self.JointType)

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Joint, self).writePrivateData(writer) :
            return False


        writer.moveIn("MatrixTransform TRUE")
        writer.moveIn("osg::MatrixTransform")
        writer.writeLine( "UniqueID %s" % str(self.TransformUniqueID) )
        writer.moveOut("osg::MatrixTransform")
        writer.moveOut("MatrixTransform TRUE")



        if self.Body1:
            writer.moveIn("Body1 TRUE")
            writer.moveIn("osgODE::RigidBody")
            writer.writeLine("UniqueID %d" %self.Data.RigidBodyCache.get(self.Body1)) ;
            writer.moveOut("osgODE::RigidBody")
            writer.moveOut("Body1 TRUE")


        if self.Body2:
            writer.moveIn("Body2 TRUE")
            writer.moveIn("osgODE::RigidBody")
            writer.writeLine("UniqueID %d" %self.Data.RigidBodyCache.get(self.Body2)) ;
            writer.moveOut("osgODE::RigidBody")
            writer.moveOut("Body2 TRUE")



        writer.writeLine("Anchor1 %f %f %f" %(self.Anchor1.x, self.Anchor1.y, self.Anchor1.z))
        writer.writeLine("Anchor2 %f %f %f" %(self.Anchor2.x, self.Anchor2.y, self.Anchor2.z))

        writer.writeLine("Axis1 %f %f %f" %(self.Axis1.x, self.Axis1.y, self.Axis1.z))
        writer.writeLine("Axis2 %f %f %f" %(self.Axis2.x, self.Axis2.y, self.Axis2.z))
        writer.writeLine("Axis3 %f %f %f" %(self.Axis3.x, self.Axis3.y, self.Axis3.z))



        if self.LoStop != None:
            writer.writeLine("dParamLoStop %f" %self.LoStop)

        if self.HiStop != None:
            writer.writeLine("dParamHiStop %f" %self.HiStop)



        if self.LoStop2 != None:
            writer.writeLine("dParamLoStop2 %f" %self.LoStop2)

        if self.HiStop2 != None:
            writer.writeLine("dParamHiStop2 %f" %self.HiStop2)



        if self.LoStop3 != None:
            writer.writeLine("dParamLoStop3 %f" %self.LoStop3)

        if self.HiStop3 != None:
            writer.writeLine("dParamHiStop3 %f" %self.HiStop3)



        if self.Axis1Anchor != None:
            writer.writeLine("Axis1Anchor %d" %self.Axis1Anchor)

        if self.Axis2Anchor != None:
            writer.writeLine("Axis2Anchor %d" %self.Axis2Anchor)

        if self.Axis3Anchor != None:
            writer.writeLine("Axis3Anchor %d" %self.Axis3Anchor)


        if self.MotorMode != None:
            writer.writeLine("MotorMode %d" %self.MotorMode)

        if self.NumAxes != None:
            writer.writeLine("NumAxes %d" %self.NumAxes)



        return True
############################################################################




############################################################################
    def selectJointTypeBy6DOF(self, constraint):
        super(Joint, self).buildGraph()




        #
        # need amplus + lmplus here
        #



        lx = constraint.use_limit_x
        ly = constraint.use_limit_y
        lz = constraint.use_limit_z

        ax = constraint.use_angular_limit_x
        ay = constraint.use_angular_limit_y
        az = constraint.use_angular_limit_z

        l3 = lx and ly and lz
        a3 = ax and ay and az

        xm = constraint.limit_min_x
        xM = constraint.limit_max_x
        ym = constraint.limit_min_y
        yM = constraint.limit_max_y
        zm = constraint.limit_min_z
        zM = constraint.limit_max_z

        am = constraint.limit_angle_min_x
        aM = constraint.limit_angle_max_x
        bm = constraint.limit_angle_min_y
        bM = constraint.limit_angle_max_y
        cm = constraint.limit_angle_min_z
        cM = constraint.limit_angle_max_z

        xl = (xm == xM)
        yl = (ym == yM)
        zl = (zm == zM)

        ll = xl and yl and zl and (xm == 0) and (ym == 0) and (zm == 0)
        al = (am == aM) and (bm == bM) and (cm == cM) and (am == 0) and (bm == 0) and (cm == 0)


        joint_type = None
        axis_order = (0, 1, 2)
        lo_stop = -3.40282e+38
        hi_stop =  3.40282e+38

        err = True



        if l3 and a3 and ll and al :
            joint_type = "osgODE::FixedJoint"
            axis_order = (0, 1, 2)
            err = False


        elif not(lx or ly or lz) and ax and ay:
            joint_type = "osgODE::UniversalJoint"
            axis_order = (0,1,2)

            self.LoStop = am
            self.HiStop = aM

            self.LoStop2 = bm
            self.HiStop2 = bM


            err = False


        elif not(lx or ly or lz) and (ax or ay or az):
            joint_type = "osgODE::AMotorJoint"
            axis_order = (0,1,2)

            self.MotorMode = 1
            self.NumAxes = 3

            self.Axis1Anchor = 1
            self.Axis2Anchor = 0

            if not self.Object.target:
                self.Axis3Anchor = 1
            else:
                self.Axis3Anchor = 2

            if ax:
                self.LoStop = am
                self.HiStop = aM

            if ay:
                self.LoStop2 = bm
                self.HiStop2 = bM

            if az:
                self.LoStop3 = cm
                self.HiStop3 = cM


            err = False



        if err:
            self.Data.Operator.report({'ERROR'}, "Unsupported 6DOF type")


        return joint_type, axis_order
############################################################################




# ........................................................................ #
############################################################################
