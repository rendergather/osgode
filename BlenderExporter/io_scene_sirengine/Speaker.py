# -*- coding: iso-8859-1 -*-
# file Speaker.py
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

import bpy
from mathutils import Vector, Matrix
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Speaker(Writable.Writable):
    """pSound::Speaker"""





############################################################################
    Object = None
    CullingActive = None
    AutoPlay = None
    AutoComputeVelocity = None
    PITCH = None
    GAIN = None
    MAX_DISTANCE = None
    ROLLOFF_FACTOR = None
    REFERENCE_DISTANCE = None
    MIN_GAIN = None
    MAX_GAIN = None
    CONE_OUTER_GAIN = None
    CONE_INNER_ANGLE = None
    CONE_OUTER_ANGLE = None
    SEC_OFFSET = None
    LOOPING = None
    POSITION = None
    VELOCITY = None
    DIRECTION = None
    BufferID = None
    FilePath = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Speaker, self).__init__(data)

        self.Object = obj
        self.CullingActive = False
        self.AutoPlay = True
        self.AutoComputeVelocity = False
        self.PITCH = 1.0
        self.GAIN = 1.0
        self.MAX_DISTANCE = 3.40282e+38
        self.ROLLOFF_FACTOR = 1.0
        self.REFERENCE_DISTANCE = 1.0
        self.MIN_GAIN = 0.0
        self.MAX_GAIN = 1.0
        self.CONE_OUTER_GAIN = 0.0
        self.CONE_INNER_ANGLE = 360.0
        self.CONE_OUTER_ANGLE = 360.0
        self.SEC_OFFSET = 0.0
        self.LOOPING = True
        self.POSITION = [0.0, 0.0, 0.0]
        self.VELOCITY = [0.0, 0.0, 0.0]
        self.DIRECTION = [0.0, 0.0, -1.0]
        self.BufferID = None
        self.FilePath = None
############################################################################




############################################################################
    def buildGraph(self):
        if not super(Speaker, self).buildGraph():
            return False



        speaker = self.Object.data


        self.CullingActive = False
        self.PITCH = speaker.volume
        self.GAIN = speaker.pitch
        self.MAX_DISTANCE = speaker.distance_max
        self.ROLLOFF_FACTOR = 1.0
        self.REFERENCE_DISTANCE = speaker.distance_reference
        self.MIN_GAIN = speaker.volume_min
        self.MAX_GAIN = speaker.volume_max
        self.CONE_OUTER_GAIN = speaker.cone_volume_outer
        self.CONE_INNER_ANGLE = speaker.cone_angle_inner
        self.CONE_OUTER_ANGLE = speaker.cone_angle_outer
        self.LOOPING = True




        self.VELOCITY = [0.0, 0.0, 0.0]


        p, q, s = self.Object.matrix_local.decompose()

        d = Vector()
        d.z = -1.0

        d = q * d

        self.POSITION = [p.x, p.y, p.z]
        self.DIRECTION = [d.x, d.y, d.z]




        if speaker.sound:
            if self.Data.Cache.has(speaker.sound):
                self.BufferID = self.Data.Cache.get(speaker.sound)
            else:
                self.BufferID = self.Data.UniqueID.generate()
                self.FilePath = bpy.path.relpath(speaker.sound.filepath).replace("//", "").replace("\\", "/")
                self.Data.Cache.set(speaker.sound, self.BufferID)



        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("pSound::Source") ;

        if not self.writePrivateData(writer):
            return False

        writer.moveOut("pSound::Source")

        return True
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(Speaker, self).writeToStream(writer) :
            return False



        if self.CullingActive:
            writer.writeLine("CullingActive TRUE")
        else:
            writer.writeLine("CullingActive FALSE")

        if self.AutoPlay:
            writer.writeLine("AutoPlay TRUE")
        else:
            writer.writeLine("AutoPlay FALSE")

        if self.AutoComputeVelocity:
            writer.writeLine("AutoComputeVelocity TRUE")
        else:
            writer.writeLine("AutoComputeVelocity FALSE")


        writer.writeLine("PITCH %f" %self.PITCH)
        writer.writeLine("GAIN %f" %self.GAIN)
        writer.writeLine("MAX_DISTANCE %e" %self.MAX_DISTANCE)
        writer.writeLine("ROLLOFF_FACTOR %f" %self.ROLLOFF_FACTOR)
        writer.writeLine("REFERENCE_DISTANCE %f" %self.REFERENCE_DISTANCE)
        writer.writeLine("MIN_GAIN %f" %self.MIN_GAIN)
        writer.writeLine("MAX_GAIN %f" %self.MAX_GAIN)
        writer.writeLine("CONE_OUTER_GAIN %f" %self.CONE_OUTER_GAIN)
        writer.writeLine("CONE_INNER_ANGLE %f" %self.CONE_INNER_ANGLE)
        writer.writeLine("CONE_OUTER_ANGLE %f" %self.CONE_OUTER_ANGLE)
        writer.writeLine("SEC_OFFSET %f" %self.SEC_OFFSET)

        if self.LOOPING:
            writer.writeLine("LOOPING 1")
        else:
            writer.writeLine("LOOPING 0")

        writer.writeLine("POSITION 0.0 0.0 0.0")
        writer.writeLine("VELOCITY %f %f %f" %(self.VELOCITY[0], self.VELOCITY[1], self.VELOCITY[2]))
        writer.writeLine("DIRECTION 0.0 0.0 -1.0")


        writer.writeLine("Position %f %f %f" %(self.POSITION[0], self.POSITION[1], self.POSITION[2]))
        writer.writeLine("Direction %f %f %f" %(self.DIRECTION[0], self.DIRECTION[1], self.DIRECTION[2]))



        if self.BufferID:
            writer.moveIn("Buffer pSound::Buffer")

            writer.writeLine("UniqueID %d" %self.BufferID)

            if self.FilePath:
                writer.writeLine("SoundFileName \"%s\"" %self.FilePath)

            writer.moveOut("Buffer pSound::Buffer")


        return True
############################################################################




# ........................................................................ #
############################################################################
