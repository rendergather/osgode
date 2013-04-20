/*!
 * @file CharacterBase_serializer.cpp
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
#include <osgODE/CharacterBase>
#include <osgODE/RigidBody>
#include <osgODE/AMotorJoint>
#include <osgODE/LMotorJoint>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( CharacterBase,
                         new osgODE::CharacterBase,
                         osgODE::CharacterBase,
                         "osg::Object osgODE::ODEObject osgODE::Container osgODE::CharacterBase" )
{

    ADD_OBJECT_SERIALIZER( Body, osgODE::RigidBody, NULL ) ;
    ADD_OBJECT_SERIALIZER( AngularMotor, osgODE::AMotorJoint, NULL ) ;
    ADD_OBJECT_SERIALIZER( LinearMotor, osgODE::LMotorJoint, NULL ) ;
    ADD_VEC3_SERIALIZER( UpVersor, osg::Z_AXIS ) ;
    ADD_VEC3_SERIALIZER( SideVersor, osg::X_AXIS ) ;
    ADD_VEC3_SERIALIZER( FrontVersor, -osg::Z_AXIS ) ;
    ADD_DOUBLE_SERIALIZER( Yaw, 0.0 ) ;
    ADD_DOUBLE_SERIALIZER( Pitch, osg::PI * 0.5 ) ;
    ADD_UINT_SERIALIZER( Elasticity, 5 ) ;
    ADD_DOUBLE_SERIALIZER( Height, 1.75 ) ;
    ADD_DOUBLE_SERIALIZER( FootContactSpring, 10000 ) ;
    ADD_DOUBLE_SERIALIZER( FootContactDamper, 1500 ) ;
}
/* ....................................................................... */
/* ======================================================================= */
