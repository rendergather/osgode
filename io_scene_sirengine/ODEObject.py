# -*- coding: iso-8859-1 -*-
# file ODEObject.py
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
from . import Writable
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class ODEObject(Writable.Writable):
    """osgODE::ODEObject"""





############################################################################
    Object = None
    ID = 0

    UserValues = []

    UpdateCallback = None

    IsCharacter = False

    Name = ""
############################################################################









############################################################################
    def __init__(self, data, obj):
        super(ODEObject, self).__init__(data)

        self.Object = obj
        self.ID = 0
        self.UserValues = []
        self.UpdateCallback = None
        self.Name = ""
        self.IsCharacter = False
############################################################################




############################################################################
    def buildGraph(self):
        super(ODEObject, self).buildGraph()

        self.ID = self.Data.ODEID.generate()

        self.Name = "%s@%s" %(self.__class__.__name__, self.Object.name)

        return True
############################################################################




############################################################################
    def writeToStream(self, writer):

        writer.moveIn("osgODE::ODEObject") ;

        self.writePrivateData(writer)

        writer.moveOut("osgODE::ODEObject")

        return True
#########################################################################




############################################################################
    def addUserValue(self, pName, pValue, pType="String"):
        self.UserValues.append( ( str(pName), str(pValue), str(pType) ) )
############################################################################




############################################################################
    def getUserValue(self, pName):
        for uv in self.UserValues:
            if pName == uv[0]:
                return (uv[0], uv[1], uv[2])
        return None
############################################################################




############################################################################
    def writePrivateData(self, writer):

        if not super(ODEObject, self).writeToStream(writer) :
            return False





        writer.writeLine("Name \"%s\"" %(self.Name))




        if self.UserValues != [] :
            writer.moveIn( "UserDataContainer TRUE" )
            writer.moveIn( "osg::DefaultUserDataContainer" )



            writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() )

            writer.moveIn( "UDC_UserObjects %d" %len(self.UserValues) )


            for i in self.UserValues:
                writer.moveIn( "osg::%sValueObject" %i[2] ) ;

                writer.writeLine( "UniqueID %d" % self.Data.UniqueID.generate() ) ;

                writer.writeLine( "Name \"%s\"" % str(i[0]) ) ;


                if i[2] == "String":
                    writer.writeLine( "Value \"%s\"" % str(i[1]) ) ;
                else:
                    writer.writeLine( "Value %s" % str(i[1]) ) ;

                writer.moveOut( "osg::%sValueObject" %i[2] ) ;


            writer.moveOut( "UDC_UserObjects %d" %len(self.UserValues) )



            writer.moveOut( "osg::DefaultUserDataContainer" )
            writer.moveOut( "UserDataContainer TRUE" )


        writer.writeLine("ID %s" %self.ID)


        if self.UpdateCallback:
                writer.moveIn("UpdateCallback TRUE")
                self.UpdateCallback.writeToStream(writer)
                writer.moveOut("UpdateCallback TRUE")


        return True
############################################################################




############################################################################
    def addUpdateCallback(self, cbk):
        if self.UpdateCallback:
            self.UpdateCallback.addNestedCallback( cbk )
        else:
            self.UpdateCallback = cbk
############################################################################




# ........................................................................ #
############################################################################
