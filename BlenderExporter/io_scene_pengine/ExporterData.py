# -*- coding: iso-8859-1 -*-
# file ExporterData.py
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
    Operator = None
    UniqueID = None
    VertexID = None
    ODEID = None
    Cache = None
    RigidBodyCache = None
    MasterStateSet = None
    Selected = None
    ExportSelected = False
    ExportLights = True
    WorldStep = None
############################################################################









############################################################################
    def __init__(self):
        self.FilePath = "untitled.osgt"
        self.Context = None
        self.Operator = None
        self.UniqueID = None
        self.VertexID = None
        self.ODEID = None
        self.Cache = None
        self.RigidBodyCache = None
        self.MasterStateSet = None
        self.Selected = None
        self.ExportSelected = False
        self.ExportLights = True
        self.WorldStep = None
############################################################################




# ........................................................................ #
############################################################################
