# -*- coding: iso-8859-1 -*-
# file SceneGraph.py
# author Rocco Martino
#
############################################################################
#    Copyright (C) 2012 - 2014 by Rocco Martino                            #
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
from . import Root, ExporterData
import bpy
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class SceneGraph(object):
    """The exported scene graph"""





############################################################################
    Data = None
    Roots = None
    RootID = None
############################################################################









############################################################################
    def __init__(self, data):
        self.Data = data
        self.Roots = []
        self.RootID = None
############################################################################




############################################################################
    def checkObjects(self):
        return True
############################################################################




############################################################################
    def buildGraph(self):

        self.RootID = self.Data.UniqueID.generate()



        scenes = None


        if self.Data.ExportCurrentScene:
            scenes = [ self.Data.Context.scene ]
        else:
            scenes = bpy.data.scenes




        for scene in scenes:
            data = ExporterData.ExporterData( self.Data )
            data.Scene = scene

            self.Roots.append ( Root.Root(data) )

        for root in self.Roots:
            if not root.buildGraph():
                return False

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        if not self.Data.ExportSelected:

            if len(self.Roots) == 1:
                return self.Roots[0].writeToStream(writer)


            else:
                writer.moveIn("osg::Group")

                writer.writeLine("UniqueID %u" % self.RootID)
                writer.writeLine("Name \"%s\"" % "SceneGraph")

                writer.moveIn("Children %u" % len(self.Roots))

                for root in self.Roots:
                    if not root.writeToStream(writer):
                        return False

                writer.moveOut("Children %u" % len(self.Roots))


                writer.moveOut("osg::Group")

        else:
            for root in self.Roots:
                if root.Data.Selected:
                    return root.Data.Selected.writeToStream(writer)

        return False
############################################################################




# ........................................................................ #
############################################################################
