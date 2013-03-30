/*!
 * @file Manager_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Manager>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Manager,
                         new osgODE::Manager,
                         osgODE::Manager,
                         "osg::Object osg::Node osgODE::Manager" )
{
    ADD_DOUBLE_SERIALIZER(StepSize, 1.0/60.0) ;
    ADD_DOUBLE_SERIALIZER(TimeMultiplier, 1.0) ;
    ADD_BOOL_SERIALIZER(Done, false) ;
    ADD_BOOL_SERIALIZER(AutoStartThread, false) ;
    ADD_BOOL_SERIALIZER(AcceptVisitors, false) ;
    ADD_BOOL_SERIALIZER(ForceUpdateTraversal, false) ;
    ADD_OBJECT_SERIALIZER(World, osgODE::World, NULL) ;
}
/* ....................................................................... */
/* ======================================================================= */
