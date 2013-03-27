/*!
 * @file LMotorJoint_serializer.cpp
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
#include <osgODE/LMotorJoint>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool
checkAxis1Anchor(const osgODE::LMotorJoint& joint)
{
    (void) joint ;
    return true ;
}

static bool
writeAxis1Anchor(osgDB::OutputStream& os, const osgODE::LMotorJoint& joint)
{
    os << joint.getAxis1Anchor() << std::endl ;
    return true ;
}

static bool
readAxis1Anchor(osgDB::InputStream& is, osgODE::LMotorJoint& joint)
{
    int a ;
    is >> a ;
    joint.setAxis1Anchor((osgODE::LMotorJoint::AxisAnchor) a) ;
    return true ;
}



static bool
checkAxis2Anchor(const osgODE::LMotorJoint& joint)
{
    (void) joint ;
    return true ;
}

static bool
writeAxis2Anchor(osgDB::OutputStream& os, const osgODE::LMotorJoint& joint)
{
    os << joint.getAxis2Anchor() << std::endl ;
    return true ;
}

static bool
readAxis2Anchor(osgDB::InputStream& is, osgODE::LMotorJoint& joint)
{
    int a ;
    is >> a ;
    joint.setAxis2Anchor((osgODE::LMotorJoint::AxisAnchor) a) ;
    return true ;
}



static bool
checkAxis3Anchor(const osgODE::LMotorJoint& joint)
{
    (void) joint ;
    return true ;
}

static bool
writeAxis3Anchor(osgDB::OutputStream& os, const osgODE::LMotorJoint& joint)
{
    os << joint.getAxis3Anchor() << std::endl ;
    return true ;
}

static bool
readAxis3Anchor(osgDB::InputStream& is, osgODE::LMotorJoint& joint)
{
    int a ;
    is >> a ;
    joint.setAxis3Anchor((osgODE::LMotorJoint::AxisAnchor) a) ;
    return true ;
}
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( LMotorJoint,
                         new osgODE::LMotorJoint,
                         osgODE::LMotorJoint,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::Joint osgODE::LMotorJoint" )
{
    ADD_USER_SERIALIZER( Axis1Anchor ) ;
    ADD_USER_SERIALIZER( Axis2Anchor ) ;
    ADD_USER_SERIALIZER( Axis3Anchor ) ;
    ADD_UINT_SERIALIZER( NumAxes, 3 ) ;
}
/* ....................................................................... */
/* ======================================================================= */
