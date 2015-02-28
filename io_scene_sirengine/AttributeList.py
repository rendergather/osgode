# -*- coding: iso-8859-1 -*-
# file AttributeList.py
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
import math
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class AttributeList(Writable.Writable):
    """AttributeList in the scene"""





############################################################################
    Object = None
    Attributes = []
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(AttributeList, self).__init__(data)

        self.Object = obj
        self.Attributes = []
############################################################################




############################################################################
    def buildGraph(self):
        super(AttributeList, self).buildGraph()

        polygon_offset = 0.0 ;

        try:
            polygon_offset = self.Object["oo_polygon_offset"] * self.Data.PolygonOffsetMultiplier
        except:
            polygon_offset = 0.0



        transparency = self.Object.data.materials[0] and self.Object.data.materials[0].use_transparency and not self.Data.ExportViewer



        if polygon_offset > 0.0:
            self.Attributes.append(
                (   "osg::PolygonOffset", \
                    "UniqueID %d" %self.Data.UniqueID.generate(), \
                    "Factor %f" %(math.copysign(1.0, polygon_offset) * -1.0), \
                    "Units %f" %(polygon_offset * -1.0) \
                ) \
            )



        if transparency:
            self.Attributes.append(
                (   "osg::Depth", \
                    "UniqueID %d" %self.Data.UniqueID.generate(), \
                    "WriteMask FALSE", \
                ) \
            )


            self.Attributes.append(
                (   "osg::BlendFunc", \
                    "UniqueID %d" %self.Data.UniqueID.generate(), \
                    "SourceRGB SRC_ALPHA", \
                    "SourceAlpha SRC_ALPHA", \
                    "DestinationRGB ONE_MINUS_SRC_ALPHA ", \
                    "DestinationAlpha ONE_MINUS_SRC_ALPHA", \
                ) \
            )


        return self.traverseBuild()
############################################################################




############################################################################
    def writeToStream(self, writer):
        num_attributes = len(self.Attributes)


        if num_attributes:

            writer.moveIn("AttributeList %d" % num_attributes)

            for attribute in self.Attributes:
                writer.moveIn(attribute[0])

                for i in attribute[1:]:
                    writer.writeLine( i )

                writer.moveOut(attribute[0])

                writer.writeLine("Value OFF")


            writer.moveOut("AttributeList %d" % num_attributes)

        return True
############################################################################




# ........................................................................ #
############################################################################
