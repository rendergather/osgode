/*!
 * @file MotionPath_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
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
#include <osgODE/MotionPath>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( MotionPath,
                         new osgODE::MotionPath,
                         osgODE::MotionPath,
                         "osg::Object osgODE::ODECallback osgODE::MotionPath" )
{
    ADD_DOUBLE_SERIALIZER( Time, -1.0 ) ;

    ADD_BOOL_SERIALIZER( RemoveWhenDone, false ) ;



    BEGIN_ENUM_SERIALIZER( Status, STOP ) ;
        ADD_ENUM_VALUE( STOP ) ;
        ADD_ENUM_VALUE( PLAY ) ;
        ADD_ENUM_VALUE( PAUSE ) ;
    END_ENUM_SERIALIZER() ;


    ADD_OBJECT_SERIALIZER( PositionInterpolator, osgODE::Vec3Interpolator, NULL ) ;
    ADD_OBJECT_SERIALIZER( QuaternionInterpolator, osgODE::QuatInterpolator, NULL ) ;
}
/* ....................................................................... */
/* ======================================================================= */
