# -*- coding: iso-8859-1 -*-
# file MotionPath.py
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

import bpy
from mathutils import Quaternion, Euler
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class MotionPath(Writable.Writable):
    """osgODE::MotionPath"""





############################################################################
    Object = None
    PositionPath = None
    QuaternionPath = None
    PositionPathID = 0
    QuaternionPathID = 0
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(MotionPath, self).__init__(data)

        self.Object = obj
        self.PositionPath = [  [], [], [], []  ]
        self.QuaternionPath = [  [], [], [], [], []  ]
        self.PositionPathID = 0
        self.QuaternionPathID = 0
############################################################################




############################################################################
    def buildGraph(self):
        super(MotionPath, self).buildGraph()


        self.PositionPathID = self.Data.UniqueID.generate()
        self.QuaternionPathID = self.Data.UniqueID.generate()



        self.PositionPath = [  [], [], [], []  ]
        self.QuaternionPath = [  [], [], [], [], []  ]

        animation_data = self.Object.animation_data
        action = animation_data.action
        fcurves = action.fcurves



        location_times = []
        rotation_times = []


        for curve in fcurves:

            if curve.data_path == "location":
                for kf in curve.keyframe_points:
                    co = kf.co
                    if not co[0] in location_times:
                        location_times.append(co[0])

            elif curve.data_path in ["rotation_euler", "rotation_quaternion"]:
                for kf in curve.keyframe_points:
                    co = kf.co
                    if not co[0] in rotation_times:
                        rotation_times.append(co[0])


        location_times.sort()
        rotation_times.sort()

        self.PositionPath[0] = location_times
        self.QuaternionPath[0] = rotation_times

        euler_path = [ [], [], [] ]
        euler_used = False

        for curve in fcurves:

            if curve.data_path == "location":
                for t in location_times:
                    self.PositionPath[ curve.array_index + 1 ].append( curve.evaluate(t) )

            elif curve.data_path == "rotation_quaternion":
                for t in rotation_times:
                    idx = curve.array_index

                    if idx == 0:
                        idx = 4

                    self.QuaternionPath[ idx ].append( curve.evaluate(t) )



            elif curve.data_path == "rotation_euler":
                euler_used = True
                for t in rotation_times:

                    euler_path[ curve.array_index ].append( curve.evaluate(t) )



        if euler_used:
            for i in range(0, len(rotation_times), 1) :
                e = Euler()
                e.x = euler_path[0][i]
                e.y = euler_path[1][i]
                e.z = euler_path[2][i]

                q = e.to_quaternion()

                self.QuaternionPath[1].append( q.x )
                self.QuaternionPath[2].append( q.y )
                self.QuaternionPath[3].append( q.z )
                self.QuaternionPath[4].append( q.w )



        for i in range( 0, len(self.PositionPath[0]), 1):
            self.PositionPath[0][i] = self.PositionPath[0][i] / bpy.data.scenes[0].render.fps

        for i in range( 0, len(self.QuaternionPath[0]), 1):
            self.QuaternionPath[0][i] = self.QuaternionPath[0][i] / bpy.data.scenes[0].render.fps
            #self.QuaternionPath[1][i] = self.QuaternionPath[1][i]
            #self.QuaternionPath[2][i] = self.QuaternionPath[2][i]
            #self.QuaternionPath[3][i] = self.QuaternionPath[3][i]
            #self.QuaternionPath[4][i] = self.QuaternionPath[4][i]



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::MotionPath") ;

        self.writePrivateData(writer)

        writer.moveOut("osgODE::MotionPath")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(MotionPath, self).writeToStream(writer) :
            return False


        writer.writeLine("Name \"%s@%s\"" %(self.__class__.__name__, self.Object.name))

        writer.writeLine("Status PLAY")


        writer.moveIn("PositionInterpolator TRUE")
        writer.moveIn("osgODE::LinearVec3Interpolator")

        writer.writeLine( "UniqueID %s" % str(self.PositionPathID) )
        writer.writeLine( "Looping FALSE" )


        writer.moveIn("PointList %d" % len(self.PositionPath[0]))

        for i in range( 0, len(self.PositionPath[0]), 1):
            writer.writeLine("%f %f %f %f" %( self.PositionPath[0][i], self.PositionPath[1][i], self.PositionPath[2][i], self.PositionPath[3][i] ) )

        writer.moveOut("PointList %d" % len(self.PositionPath[0]))


        writer.moveOut("osgODE::LinearVec3Interpolator")
        writer.moveOut("PositionInterpolator TRUE")





        writer.moveIn("QuaternionInterpolator TRUE")
        writer.moveIn("osgODE::LinearQuatInterpolator")

        writer.writeLine( "UniqueID %s" % str(self.QuaternionPathID) )
        writer.writeLine( "Looping FALSE" )


        writer.moveIn("PointList %d" % len(self.QuaternionPath[0]))

        for i in range( 0, len(self.QuaternionPath[0]), 1):
            writer.writeLine("%f %f %f %f %f" %( self.QuaternionPath[0][i], self.QuaternionPath[1][i], self.QuaternionPath[2][i], self.QuaternionPath[3][i], self.QuaternionPath[4][i] ) )

        writer.moveOut("PointList %d" % len(self.QuaternionPath[0]))


        writer.moveOut("osgODE::LinearQuatInterpolator")
        writer.moveOut("QuaternionInterpolator TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################
