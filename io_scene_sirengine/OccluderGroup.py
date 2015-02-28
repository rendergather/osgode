# -*- coding: iso-8859-1 -*-
# file OccluderGroup.py
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
class OccluderGroup(Writable.Writable):
    """OccluderGroup group in the scene"""





############################################################################
############################################################################









############################################################################
    def __init__(self, data):
        super(OccluderGroup, self).__init__(data)
############################################################################




############################################################################
    def buildGraph(self):
        super(OccluderGroup, self).buildGraph()


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



            is_physics = True

            if num_materials == 0:
                is_physics = False
            elif not obj.material_slots[0].material:
                is_physics = False
            elif not obj.material_slots[0].material.game_settings.physics:
                is_physics = False


            if is_physics and obj.parent == None and obj.game.physics_type == 'OCCLUDE':
                from . import OccluderNode
                occluder = OccluderNode.OccluderNode(self.Data, obj)

                self.addChild(occluder)


                if self.Data.Context.selected_objects[0] == obj:
                    self.Data.Selected = occluder



        return True
############################################################################




############################################################################
    def writeToStream(self, writer):
        writer.moveIn("osg::Group") ;

        if not super(OccluderGroup, self).writeToStream(writer) :
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
