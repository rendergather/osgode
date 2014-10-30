# -*- coding: iso-8859-1 -*-
# file __init__.py
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
import bpy

from bpy_extras.io_utils import ExportHelper

from bpy.props import StringProperty, BoolProperty, EnumProperty, FloatProperty


if "osgt_exporter" in locals():
    import imp
    imp.reload(osgt_exporter)
############################################################################




############################################################################
# ........................................................................ #
bl_info = {
    "name"          :   "SirEngine osgt format",
    "author"        :   "Rocco Martino",
    "blender"       :   (2, 6, 2),
    "location"      :   "File > Export",
    "description"   :   "Export meshes and game physics",
    "category"      :   "Import-Export"
}
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class ExportOSGT(bpy.types.Operator, ExportHelper):
    """Write an OpenSceneGraph OSGT Scene"""





############################################################################
    bl_idname = "export_scene.osgt"
    bl_label = "Export OSGT"

    filename_ext = ".osgt"

    filter_glob = StringProperty(
                    default = "*.osgt",
                    options = {"HIDDEN"}
                )




    threading_mode = BoolProperty( name="Compute dynamics in a separate thread", description="Experimental", default=False)

    export_selected = BoolProperty( name="Export selected only", default=False)

    export_current_scene = BoolProperty( name="Export current scene only", default=False)


    export_lights = BoolProperty( name="Export pViewer (requires pViewer)", default=False)
    export_sounds = BoolProperty( name="Export sounds (requires pSound)", default=False)
    export_game   = BoolProperty( name="Export game (requiers ooGame)", default=False)


    max_anisotropy = FloatProperty( name="Max anisotropy", default=4, min=0)


    ambient_multiplier = FloatProperty( name="Ambient light multiplier", default=1.0, min=0.0)


    polygon_offset_multiplier = FloatProperty( name="PolygonOffset multiplier", default=1.0, min=0.0)


    stop_erp = FloatProperty( name="Stop ERP", default=0.2, min=0.0, max=1.0)
    stop_cfm = FloatProperty( name="Stop CFM", default=1.0e-5, min=0.0, max=1.0)


    contact_surface_layer = FloatProperty( name="Contact surface layer", default=0.005, min=0.0, max=1.0)



    world_step = EnumProperty(  items = (
                                    ("dWorldStep", "dWorldStep", "Accuracy"),
                                    ("dWorldQuickStep", "dWorldQuickStep", "Performance")
                                ),
                                name = "World step",
                                description = "Select a world step function",
                                default = "dWorldQuickStep" )
############################################################################









############################################################################
    def execute(self, context):

        from . import Exporter, ExporterData, IDGenerator, Cache

        data = ExporterData.ExporterData()
        exporter = Exporter.Exporter(data)

        data.FilePath = self.filepath
        data.Context = context
        data.Scene = data.Context.scene
        data.Operator = self
        data.Threaded = self.threading_mode
        data.UniqueID = IDGenerator.IDGenerator()
        data.VertexID = IDGenerator.IDGenerator()
        data.ODEID = IDGenerator.IDGenerator()
        data.Cache = Cache.Cache()
        data.RigidBodyCache = Cache.Cache()
        data.RigidBodyMap = {}
        data.RigidBodyNames = {}
        data.ExportSelected = self.export_selected
        data.ExportCurrentScene = self.export_current_scene
        data.ExportLights = self.export_lights
        data.ExportSounds = self.export_sounds
        data.ExportGame = self.export_game
        data.ContactSurfaceLayer = self.contact_surface_layer
        data.WorldStep = self.world_step
        data.MaxAnisotropy = self.max_anisotropy
        data.AmbientMultiplier = self.ambient_multiplier
        data.PolygonOffsetMultiplier = self.polygon_offset_multiplier
        data.StopERP = self.stop_erp
        data.StopCFM = self.stop_cfm

        return exporter.export()
############################################################################




# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
def menu_func_export(self, context):
    self.layout.operator(ExportOSGT.bl_idname, text = "SirEngine (.osgt)")




def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)




def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)




if __name__ == "__main__":
    register()
# ........................................................................ #
############################################################################
