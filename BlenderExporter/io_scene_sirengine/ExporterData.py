# -*- coding: iso-8859-1 -*-
# file ExporterData.py
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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class ExporterData(object):
    """Data passed to the Exporter"""





############################################################################
    FilePath = "untitled.osgt"
    Context = None
    Scene = None
    Operator = None
    Threaded = None
    UniqueID = None
    VertexID = None
    ODEID = None
    Cache = None
    RigidBodyCache = None
    RigidBodyMap = None
    RigidBodyNames = None
    MasterStateSet = None
    Selected = None
    ExportSelected = False
    ExportCurrentScene = False
    ExportLights = False
    ExportSounds = False
    ExportGame = False
    ContactSurfaceLayer = None
    WorldStep = None
    MaxAnisotropy = None
    AmbientMultiplier = None
    PolygonOffsetMultiplier = None
    StopERP = None
    StopCFM = None
############################################################################









############################################################################
    def __init__(self, data=None):

        if data:
            self.FilePath = data.FilePath
            self.Context = data.Context
            self.Scene = data.Scene
            self.Threaded = data.Threaded
            self.Operator = data.Operator
            self.UniqueID = data.UniqueID
            self.VertexID = data.VertexID
            self.ODEID = data.ODEID
            self.Cache = data.Cache
            self.RigidBodyCache = data.RigidBodyCache
            self.RigidBodyMap = data.RigidBodyMap
            self.RigidBodyNames = data.RigidBodyNames
            self.MasterStateSet = data.MasterStateSet
            self.Selected = data.Selected
            self.ExportSelected = data.ExportSelected
            self.ExportCurrentScene = data.ExportCurrentScene
            self.ExportLights = data.ExportLights
            self.ExportSounds = data.ExportSounds
            self.ExportGame = data.ExportGame
            self.ContactSurfaceLayer = data.ContactSurfaceLayer
            self.WorldStep = data.WorldStep
            self.MaxAnisotropy = data.MaxAnisotropy
            self.AmbientMultiplier = data.AmbientMultiplier
            self.PolygonOffsetMultiplier = data.PolygonOffsetMultiplier
            self.StopERP = data.StopERP
            self.StopCFM = data.StopCFM

        else:
            self.FilePath = "untitled.osgt"
            self.Context = None
            self.Scene = None
            self.Operator = None
            self.Threaded = None
            self.UniqueID = None
            self.VertexID = None
            self.ODEID = None
            self.Cache = None
            self.RigidBodyCache = None
            self.RigidBodyMap = None
            self.RigidBodyNames = None
            self.MasterStateSet = None
            self.Selected = None
            self.ExportSelected = False
            self.ExportCurrentScene = False
            self.ExportLights = False
            self.ExportSounds = False
            self.ExportGame = False
            self.ContactSurfaceLayer = None
            self.WorldStep = None
            self.MaxAnisotropy = None
            self.AmbientMultiplier = None
            self.PolygonOffsetMultiplier = None
            self.StopERP = None
            self.StopCFM = None
############################################################################




# ........................................................................ #
############################################################################
