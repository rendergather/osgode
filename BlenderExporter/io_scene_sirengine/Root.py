# -*- coding: iso-8859-1 -*-
# file Root.py
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
class Root(Writable.Writable):
    """Root group in the scene"""





############################################################################
    StateSet = None
    SceneName = None
############################################################################









############################################################################
    def __init__(self, data):
        super(Root, self).__init__(data)


        from . import StateSet
        self.StateSet = StateSet.StateSet(self.Data, None)

        self.SceneName = None

        self.Data.MasterStateSet = self.StateSet
############################################################################




############################################################################
    def buildGraph(self):
        super(Root, self).buildGraph()


        self.SceneName = self.Data.Scene.name


        from . import Manager

        manager = Manager.Manager(self.Data)
        self.addChild(manager)


        from . import PureGraphics

        group = PureGraphics.PureGraphics(self.Data)
        self.addChild(group)


        from . import OccluderGroup

        group = OccluderGroup.OccluderGroup(self.Data)
        self.addChild(group)


        if self.Data.ExportLights:
            from . import LightGroup

            group = LightGroup.LightGroup(self.Data)
            self.addChild(group)



        self.StateSet.ModeList.addMode("GL_NORMALIZE ON")
        self.StateSet.ModeList.addMode("GL_CULL_FACE ON")
        self.StateSet.ModeList.addMode("GL_BLEND OFF")

        if not self.Data.GL_LIGHTING:
            self.StateSet.ModeList.addMode("GL_LIGHTING OFF" )

        if self.Data.ExportLights:
            self.StateSet.UniformList.addVec4Uniform("uMaterial", [1.0, 0.0, 1.0, 0.0])
            self.StateSet.UniformList.addVec4Uniform("uColor", [0.8, 0.8, 0.8, 1.0])
            self.StateSet.UniformList.addVec4Uniform("uWaterSpeed", [0.01, 0.0, 0.0, 0.01])
            self.StateSet.UniformList.addFloatUniform("uIOR", 1.0)
            self.StateSet.UniformList.addFloatUniform("uParallaxScale", 0.0)


        self.StateSet.buildGraph()



        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):

        if self.Data.ExportLights:
            writer.moveIn("pViewer::Root")
        else:
            writer.moveIn("osg::Group")

        if not super(Root, self).writeToStream(writer) :
            return False


        if self.SceneName:
            writer.writeLine("Name \"%s\"" % self.SceneName)







        writer.moveIn( "UserDataContainer TRUE" )
        writer.moveIn( "osg::DefaultUserDataContainer" )



        writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() )

        writer.moveIn( "UDC_UserObjects 1" )


        writer.moveIn( "osg::BoolValueObject" ) ;

        writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() ) ;

        writer.writeLine( "Name \"oo_scene\"" ) ;

        writer.writeLine( "Value TRUE" ) ;

        writer.moveOut( "osg::BoolValueObject" ) ;


        writer.moveOut( "UDC_UserObjects 1" )



        writer.moveOut( "osg::DefaultUserDataContainer" )
        writer.moveOut( "UserDataContainer TRUE" )






        if self.StateSet:
            self.StateSet.writeToStream(writer)


        num_children = len(self.Children)

        if num_children > 0 :
            writer.moveIn("Children %s" % num_children)

            if not self.traverseWrite(writer):
                return False

            writer.moveOut("Children %s" % num_children)


        if self.Data.ExportLights:
            writer.moveOut("pViewer::Root")
        else:
            writer.moveOut("osg::Group")

        return True
############################################################################




# ........................................................................ #
############################################################################
