# -*- coding: iso-8859-1 -*-
# file Light.py
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
from . import Writable, TextureAttribute

from mathutils import Vector, Matrix, Quaternion, Euler

from math import sqrt, pi
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class Light(Writable.Writable):
    """Light in the scene"""





############################################################################
    Object = None
    Position = None
    Direction = None
    Ambient = None
    Diffuse = None
    Specular = None
    ConstantAttenuation = None
    LinearAttenuation = None
    QuadraticAttenuation = None
    MaxDistance = None
    SpotExponent = None
    SpotCutoff = None
    Enabled = None
    ShadowEnabled = None
    OnlyShadow = None
    ShadowBufferSize = None
    ShadowBufferClipStart = None
    ShadowBufferClipEnd = None
    InvRadius = None
    ShadowBias = None
    ShadowFrustumSize = None
    Type = None
    LightMatrix = None
    LightTexture = None
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(Light, self).__init__(data)

        self.Object = obj
        self.Position = None
        self.Direction = None
        self.Ambient = None
        self.Diffuse = None
        self.Specular = None
        self.ConstantAttenuation = None
        self.LinearAttenuation = None
        self.QuadraticAttenuation = None
        self.MaxDistance = None
        self.SpotExponent = None
        self.SpotCutoff = None
        self.Enabled = None
        self.ShadowEnabled = None
        self.OnlyShadow = None
        self.ShadowBufferSize = None
        self.ShadowBufferClipStart = None
        self.ShadowBufferClipEnd = None
        self.InvRadius = None
        self.ShadowBias = None
        self.ShadowFrustumSize = None
        self.Type = None
        self.LightMatrix = None
        self.LightTexture = None
############################################################################




############################################################################
    def buildGraph(self):
        super(Light, self).buildGraph()

        self.Position = None
        self.Direction = None
        self.Ambient = None
        self.Diffuse = None
        self.Specular = None
        self.ConstantAttenuation = None
        self.LinearAttenuation = None
        self.QuadraticAttenuation = None
        self.MaxDistance = None
        self.SpotExponent = None
        self.SpotCutoff = None
        self.Enabled = None
        self.ShadowEnabled = None
        self.OnlyShadow = None
        self.ShadowBufferSize = None
        self.ShadowBufferClipStart = None
        self.ShadowBufferClipEnd = None
        self.InvRadius = None
        self.ShadowBias = None
        self.ShadowFrustumSize = None
        self.LightMatrix = None
        self.LightTexture = None


        try:
            self.MaxDistance = self.Object.data["oo_max_distance"]
        except:
            self.MaxDistance = -1.0


        if self.Object.data.type == "HEMI":
            self.Enabled = True
            self.ShadowEnabled = False
        else:
            #self.Enabled = self.Object.data.use_only_shadow != True
            self.Enabled = True
            self.ShadowEnabled = self.Object.data.shadow_method != "NOSHADOW"



        if self.Object.data.type in ['SUN', 'HEMI']:
            v = Vector()
            v.x = 0.0
            v.y = 0.0
            v.z = -1.0
            v = self.Object.matrix_local.to_euler().to_quaternion() * v
            v.normalize()

            self.Position = [-v.x, -v.y, -v.z, 0.0]
            self.Direction = [v.x, v.y, v.z, 0.0]

            self.Ambient = [0, 0, 0, 1]

            c = self.Object.data.color
            e = self.Object.data.energy
            self.Diffuse = [c[0] * e, c[1] * e, c[2] * e, 1.0]

            if self.Object.data.use_specular:
                self.Specular = [c[0] * e, c[1] * e, c[2] * e, 1.0]
            else:
                self.Specular = [0, 0, 0, 1]



        elif self.Object.data.type == 'POINT':
            v = Vector()
            v.x = 0.0
            v.y = 0.0
            v.z = 0.0
            v = self.Object.matrix_local  * v

            self.Position = [v.x, v.y, v.z, 1.0]
            self.Direction = [0.0, 0.0, 0.0, 1.0]

            self.Ambient = [0, 0, 0, 1]

            c = self.Object.data.color
            e = self.Object.data.energy
            self.Diffuse = [c[0] * e, c[1] * e, c[2] * e, 1.0]

            if self.Object.data.use_specular:
                self.Specular = [c[0] * e, c[1] * e, c[2] * e, 1.0]
            else:
                self.Specular = [0, 0, 0, 1]



        if self.Object.data.type == 'SPOT':

            self.OnlyShadow = self.Object.data.use_only_shadow

            v = Vector()
            v.x = 0.0
            v.y = 0.0
            v.z = 0.0
            v = self.Object.matrix_local  * v

            self.Position = [v.x, v.y, v.z, 1.0]




            v = Vector()
            v.x = 0.0
            v.y = 0.0
            v.z = -1.0
            v = self.Object.matrix_local.to_euler().to_quaternion() * v
            v.normalize()
            self.Direction = [v.x, v.y, v.z, 0.0]




            self.Ambient = [0, 0, 0, 1]

            c = self.Object.data.color
            e = self.Object.data.energy
            self.Diffuse = [c[0] * e, c[1] * e, c[2] * e, 1.0]

            if self.Object.data.use_specular:
                self.Specular = [c[0] * e, c[1] * e, c[2] * e, 1.0]
            else:
                self.Specular = [0, 0, 0, 1]



        # Manca il termine (QL/D^3) r^3



        if self.Object.data.type == 'SUN':

            self.OnlyShadow = self.Object.data.use_only_shadow




        if self.Object.data.type in ['POINT', 'SPOT']:

            if self.Object.data.use_sphere:
                self.InvRadius = 1.0 / self.Object.data.distance


            if self.Object.data.falloff_type == 'CONSTANT':
                self.ConstantAttenuation = 1
                self.LinearAttenuation = 0
                self.QuadraticAttenuation = 0

            elif self.Object.data.falloff_type == 'INVERSE_LINEAR':
                self.ConstantAttenuation = 1
                self.LinearAttenuation = 1.0 / self.Object.data.distance
                self.QuadraticAttenuation = 0

            elif self.Object.data.falloff_type == 'INVERSE_SQUARE':
                self.ConstantAttenuation = 1
                self.LinearAttenuation = 0
                self.QuadraticAttenuation = 1.0 / pow(self.Object.data.distance, 2.0)

            elif self.Object.data.falloff_type == 'LINEAR_QUADRATIC_WEIGHTED':
                self.ConstantAttenuation = 1
                self.LinearAttenuation = self.Object.data.linear_attenuation / self.Object.data.distance
                self.QuadraticAttenuation = self.Object.data.quadratic_attenuation / pow(self.Object.data.distance, 2.0)

            else:
                print ("Unsupported falloff type: %s\n" % self.Object.data.type)

            if self.Object.data.type == 'SPOT':
                self.SpotExponent = self.Object.data.spot_blend

                self.SpotCutoff = self.Object.data.spot_size * 180.0 / pi
            else:
                self.SpotExponent = 0.0
                self.SpotCutoff = 180.0

        elif self.Object.data.type in ['SUN', 'HEMI']:
            self.ConstantAttenuation = 1
            self.LinearAttenuation = 0
            self.QuadraticAttenuation = 0
            self.SpotExponent = 0.0
            self.SpotCutoff = 180.0
        else:
            print( "Unsupported light type: %s" %self.Object.data.type )



        if self.Object.data.type == "SUN":
            self.ShadowFrustumSize = self.Object.data.shadow_frustum_size





        if self.Object.data.type == 'SPOT' or self.Object.data.type == "SUN":
            self.ShadowBufferSize = self.Object.data.shadow_buffer_size
        #elif self.Object.data.type == 'SUN':
            #self.ShadowBufferSize = 2048.0
        else :
            self.ShadowBufferSize = 1024.0



        if self.Object.data.type == 'SPOT':
            if self.Object.data.use_auto_clip_start or self.Object.data.use_auto_clip_end:
                self.ShadowBufferClipStart = -1.0
                self.ShadowBufferClipEnd = -1.0
            else:
                self.ShadowBufferClipStart = self.Object.data.shadow_buffer_clip_start
                self.ShadowBufferClipEnd = self.Object.data.shadow_buffer_clip_end



        if self.Object.data.type == 'SUN':
            self.ShadowBufferClipStart = self.Object.data.shadow_buffer_clip_start
            self.ShadowBufferClipEnd = self.Object.data.shadow_buffer_clip_end



        if self.Object.data.type == 'SPOT' or self.Object.data.type == "SUN" :
            self.ShadowBias = self.Object.data.shadow_buffer_bias


        if self.Object.data.type == 'POINT':
            fast_point = False
            try:
                if self.Object.data["oo_fast_point"] != 0.0:
                    fast_point = True
            except:
                fast_point = False


            if fast_point:
                self.Type = 'FAST_POSITIONAL'

            elif self.Object.data.use_specular:
                self.Type = 'POSITIONAL'
            else:
                self.Type = 'NO_SPECULAR_POSITIONAL'


        elif self.Object.data.type == 'SPOT':
            self.Type = 'SPOT'
        elif self.Object.data.type == 'SUN':
            self.Type = 'DIRECTIONAL'
        elif self.Object.data.type == 'HEMI':
            self.Type = 'HEMI'
        else:
            self.Type = 'UNDEFINED'



        self.LightMatrix = Matrix(self.Object.matrix_local)


        #return self.traverseBuild()



        texture = self.Object.data.texture_slots[0]

        if texture:
            self.LightTexture = TextureAttribute.TextureAttribute(self.Data, texture)
            self.LightTexture.buildGraph()



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("pViewer::Light") ;

        if not super(Light, self).writeToStream(writer) :
            return False


        writer.writeLine("Name \"Light@%s\"" %self.Object.name)



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

        if self.MaxDistance != None:
            writer.writeLine("MaxDistance %f" %(self.MaxDistance))

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

        if self.OnlyShadow == True:
            writer.writeLine("OnlyShadow TRUE")
        else:
            writer.writeLine("OnlyShadow FALSE")

        if self.ShadowBufferSize != None:
            writer.writeLine("ShadowBufferSize %f" %self.ShadowBufferSize)

        if self.ShadowBufferClipStart != None:
            writer.writeLine("ShadowBufferClipStart %f" %self.ShadowBufferClipStart)

        if self.ShadowBufferClipEnd != None:
            writer.writeLine("ShadowBufferClipEnd %f" %self.ShadowBufferClipEnd)

        if self.InvRadius != None:
            writer.writeLine("InvRadius %f" %self.InvRadius)

        if self.ShadowBias != None:
            writer.writeLine("ShadowBias %f" %self.ShadowBias)

        if self.ShadowFrustumSize != None:
            writer.writeLine("ShadowFrustumSize %f" %self.ShadowFrustumSize)

        if self.Type != None:
            writer.writeLine("Type %s" %self.Type)


        if self.LightMatrix != None:
            writer.moveIn("LightMatrix")

            m = self.LightMatrix
            for i in range(4):
                writer.writeLine( "%f %f %f %f" %( m[0][i], m[1][i], m[2][i], m[3][i] ) )

            writer.moveOut("LightMatrix")


        if self.LightTexture:
            writer.moveIn("LightTexture TRUE")
            self.LightTexture.writeToStream(writer)
            writer.moveOut("LightTexture TRUE")




        writer.moveOut("pViewer::Light")

        return True
############################################################################




# ........................................................................ #
############################################################################
