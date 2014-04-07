/*!
 * @file RigidBodyServoMotor_serializer.cpp
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
#include <osgODE/RigidBodyServoMotor>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( RigidBodyServoMotor,
                         new osgODE::RigidBodyServoMotor,
                         osgODE::RigidBodyServoMotor,
                         "osg::Object osgODE::ODECallback osgODE::ServoMotor osgODE::RigidBodyServoMotor" )
{
    BEGIN_ENUM_SERIALIZER( Mode, LINEAR ) ;
        ADD_ENUM_VALUE( LINEAR ) ;
        ADD_ENUM_VALUE( ANGULAR ) ;
    END_ENUM_SERIALIZER() ;

    ADD_VEC3_SERIALIZER( Axis, osg::Vec3() ) ;

    ADD_DOUBLE_SERIALIZER( MaxForce, 0.0 ) ;

    ADD_DOUBLE_SERIALIZER( Speed, 0.0 ) ;

    ADD_BOOL_SERIALIZER( AxisLocal, true ) ;
}
/* ....................................................................... */
/* ======================================================================= */
