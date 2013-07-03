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
############################################################################




############################################################################
# ........................................................................ #
# ........................................................................ #
############################################################################




############################################################################
# ........................................................................ #
class FileWriter(object):
    """File writer helper"""





############################################################################
    Stream = None
    IndentationLevel = 0
    IndentationSize = 2

    ObjectStack = []
    CurrentObject = None
############################################################################









############################################################################
    def __init__(self):
        pass
############################################################################




############################################################################
    def open(self, file_name):
        self.Stream = open(file_name, "w")
############################################################################




############################################################################
    def close(self):
        if self.Stream:
            self.Stream.close()
            self.Stream = None
############################################################################




############################################################################
    def writeLine(self, line):
        self.Stream.write(" " * self.IndentationSize * self.IndentationLevel)
        self.Stream.write(line + "\n")
############################################################################




############################################################################
    def moveIn(self, obj_class):
        self.writeLine(obj_class + " {")

        self.IndentationLevel += 1
        self.CurrentObject = obj_class
        self.ObjectStack.append(obj_class)
############################################################################




############################################################################
    def moveOut(self, obj_class):

        if self.CurrentObject != obj_class:
            print("Error: moveout: %s, CurrentObject: %s" %(obj_class, self.CurrentObject))


        self.IndentationLevel -= 1
        self.ObjectStack.pop()

        if len(self.ObjectStack) != 0:
            self.CurrentObject = self.ObjectStack[-1]
        else:
            self.CurrentObject = None

        self.writeLine("}")
############################################################################




# ........................................................................ #
############################################################################
