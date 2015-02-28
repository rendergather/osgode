# -*- coding: iso-8859-1 -*-
# file AmbientLight.py
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class AmbientLight(Writable.Writable):
    """AmbientLight in the scene"""





############################################################################
    Position = None
    Direction = None
    Ambient = None
    Diffuse = None
    Specular = None
    ConstantAttenuation = None
    LinearAttenuation = None
    QuadraticAttenuation = None
    SpotExponent = None
    SpotCutoff = None
    Enabled = None
    ShadowEnabled = None
    Type = None
############################################################################









############################################################################
    def __init__(self, data):
        super(AmbientLight, self).__init__(data)

        self.Position = None
        self.Direction = None
        self.Ambient = None
        self.Diffuse = None
        self.Specular = None
        self.ConstantAttenuation = None
        self.LinearAttenuation = None
        self.QuadraticAttenuation = None
        self.SpotExponent = None
        self.SpotCutoff = None
        self.Enabled = None
        self.ShadowEnabled = None
        self.Type = None
############################################################################




############################################################################
    def buildGraph(self):
        super(AmbientLight, self).buildGraph()

        self.Position = [0.0, 0.0, 0.0, 0.0]
        self.Direction = [0.0, 0.0, 0.0, 0.0]
        self.Ambient = [0.0, 0.0, 0.0, 0.0]
        self.Diffuse = [0.0, 0.0, 0.0, 0.0]
        self.Specular = [0.0, 0.0, 0.0, 0.0]
        self.ConstantAttenuation = 1.0
        self.LinearAttenuation = 0.0
        self.QuadraticAttenuation = 0.0
        self.SpotExponent = 0.0
        self.SpotCutoff = 0.0
        self.Enabled = True
        self.ShadowEnabled = False




        ac = self.Data.Scene.world.ambient_color

        if ac[0] == ac[1] == ac[2] == 0.0:
            self.Ambient = [0.0, 0.0, 0.0, 0.0]
            self.Enabled = False

        else:
            self.Ambient = [ ac[0], ac[1], ac[2], 1.0 ]
            self.Enabled = True



        self.Type = 'AMBIENT'


        self.ShadowEnabled = self.Data.Scene.world.light_settings.use_ambient_occlusion






        return True

        #return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("pViewer::Light") ;

        if not super(AmbientLight, self).writeToStream(writer) :
            return False



        if self.Position != None:
            writer.writeLine("Position %f %f %f %f" %(self.Position[0], self.Position[1], self.Position[2], self.Position[3]))

        if self.Direction != None:
            writer.writeLine("Direction %f %f %f %f" %(self.Direction[0], self.Direction[1], self.Direction[2], self.Direction[3]))

        if self.Ambient != None:
            writer.writeLine("Ambient %f %f %f %f" %(self.Ambient[0], self.Ambient[1], self.Ambient[2], self.Ambient[3]))

        if self.Diffuse != None:
            writer.writeLine("Diffuse %f %f %f %f" %(self.Diffuse[0], self.Diffuse[1], self.Diffuse[2], self.Diffuse[3]))

        if self.Specular != None:
            writer.writeLine("Specular %f %f %f %f" %(self.Specular[0], self.Specular[1], self.Specular[2], self.Specular[3]))

        if self.ConstantAttenuation != None:
            writer.writeLine("ConstantAttenuation %f" %(self.ConstantAttenuation))

        if self.LinearAttenuation != None:
            writer.writeLine("LinearAttenuation %f" %(self.LinearAttenuation))

        if self.QuadraticAttenuation != None:
            writer.writeLine("QuadraticAttenuation %f" %(self.QuadraticAttenuation))

        if self.SpotExponent != None:
            writer.writeLine("SpotExponent %f" %(self.SpotExponent))

        if self.SpotCutoff != None:
            writer.writeLine("SpotCutoff %f" %(self.SpotCutoff))

        if self.Enabled != True:
            writer.writeLine("Enabled FALSE")
        else:
            writer.writeLine("Enabled TRUE")

        if self.ShadowEnabled != True:
            writer.writeLine("ShadowEnabled FALSE")
        else:
            writer.writeLine("ShadowEnabled TRUE")

        if self.Type != None:
            writer.writeLine("Type %s" %self.Type)




        writer.moveOut("pViewer::Light")

        return True
############################################################################




# ........................................................................ #
############################################################################
