# -*- coding: iso-8859-1 -*-
# file Exporter.py
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
class Exporter(object):
    """Export a scene in osgt format"""





############################################################################
    Data = None
############################################################################









############################################################################
    def __init__(self, data):
        self.Data = data
############################################################################




############################################################################
    def export(self):
        from . import SceneGraph
        from . import FileWriter



        graph = SceneGraph.SceneGraph(self.Data)

        if not graph.checkObjects():
            return {"CANCELLED"}

        if not graph.buildGraph():
            return {"CANCELLED"}


        writer = FileWriter.FileWriter()
        writer.open(self.Data.FilePath)

        writer.writeLine("#Ascii Scene")
        writer.writeLine("#Version 100")
        writer.writeLine("#Generator pExporter @PEXPORTER_VERSION@")
        writer.writeLine("")
        writer.writeLine("")

        if not graph.writeToStream(writer):
            return {"CANCELLED"}


        writer.close()


        return {"FINISHED"}
############################################################################




# ........................................................................ #
############################################################################
