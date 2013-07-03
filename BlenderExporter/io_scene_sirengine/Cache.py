# -*- coding: iso-8859-1 -*-
# file Cache.py
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
class Cache(object):





############################################################################
    Objects = {}
############################################################################









############################################################################
    def __init__(self):
        self.Objects = {}
############################################################################




############################################################################
    def has(self, obj):
        return obj in self.Objects
############################################################################




############################################################################
    def set(self, obj, unique_id):
        self.Objects[obj] = unique_id
############################################################################




############################################################################
    def get(self, obj):
        if self.has(obj):
            return self.Objects[obj]

        return None
############################################################################




# ........................................................................ #
############################################################################
