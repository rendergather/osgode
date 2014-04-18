/*!
 * @file Events_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 by Rocco Martino                                   *
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
#include <osgODE/Events>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Events,
                         new osgODE::Events,
                         osgODE::Events,
                         "osg::Object osgODE::Events" )
{
    ADD_VEC2_SERIALIZER( CursorPosition, osg::Vec2() ) ;
    ADD_VEC2_SERIALIZER( NormalizedCursorPosition, osg::Vec2() ) ;
    ADD_UINT_SERIALIZER( ButtonMask, 0 ) ;
    ADD_MATRIX_SERIALIZER( ViewMatrix, osg::Matrix() ) ;
    ADD_MATRIX_SERIALIZER( ProjectionMatrix, osg::Matrix() ) ;
}
/* ....................................................................... */
/* ======================================================================= */
