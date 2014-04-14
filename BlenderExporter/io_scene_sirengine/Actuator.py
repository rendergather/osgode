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
    Object = None
    BlenderActuator = None
    Cached = None
############################################################################









############################################################################
    def __init__(self, data, obj, actuator):
        super(Actuator, self).__init__(data)

        self.Object = obj
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

            writer.writeLine("Name \"%s@%s\"" %(self.__class__.__name__, self.Object.name))

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
    SoundSource = None
############################################################################









############################################################################
    def __init__(self, data, obj, actuator):
        super(SoundActuator, self).__init__(data, obj, actuator)

        self.Mode = None
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

        if self.SoundSource:
            writer.moveIn("Source TRUE")
            self.SoundSource.writeToStream( writer )
            writer.moveOut("Source TRUE")


        return True
############################################################################




# ........................................................................ #
############################################################################
