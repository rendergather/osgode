# -*- coding: iso-8859-1 -*-
# file PureGraphics.py
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class PureGraphics(Writable.Writable):
    """PureGraphics group in the scene"""





############################################################################
############################################################################









############################################################################
    def __init__(self, data):
        super(PureGraphics, self).__init__(data)
############################################################################




############################################################################
    def buildGraph(self):
        super(PureGraphics, self).buildGraph()


        for obj in self.Data.Context.scene.objects:
            if not self.addObject(obj):
                return False


        return self.traverseBuild()
############################################################################




############################################################################
    def addObject(self, obj):

        if obj.type == 'MESH':

            num_materials = len(obj.material_slots)

            if num_materials > 1:
                self.Data.Operator.report({'ERROR'}, "[%s] This exporter supports only 1 material per mesh" %obj.name)
                return False



            is_graphics = False

            if num_materials == 0:
                is_graphics = True
            elif not obj.material_slots[0].material:
                is_graphics = True
            elif not obj.material_slots[0].material.game_settings.physics:
                is_graphics = True


            if is_graphics and obj.parent == None:
                from . import MatrixTransform
                matrix_transform = MatrixTransform.MatrixTransform(self.Data, obj)

                self.addChild(matrix_transform)


                if self.Data.Context.selected_objects and self.Data.Context.selected_objects[0] == obj:
                    self.Data.Selected = matrix_transform




        elif obj.type == 'SPEAKER' and obj.parent == None:
            from . import Speaker
            speaker = Speaker.Speaker(self.Data, obj)

            self.addChild(speaker)


            if self.Data.Context.selected_objects and self.Data.Context.selected_objects[0] == obj:
                self.Data.Selected = speaker



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Group") ;

        if not super(PureGraphics, self).writeToStream(writer) :
            return False


        num_children = len(self.Children)

        if num_children > 0 :
            writer.moveIn("Children %s" % num_children)

            if not self.traverseWrite(writer):
                return False

            writer.moveOut("Children %s" % num_children)


        writer.moveOut("osg::Group")

        return True
############################################################################




# ........................................................................ #
############################################################################
