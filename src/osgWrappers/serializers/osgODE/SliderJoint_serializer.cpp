/*!
 * @file SliderJoint_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2014 by Rocco Martino                            *
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
#include <osgODE/SliderJoint>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
static bool checkRelativeRotation(const osgODE::SliderJoint& joint)
{
    (void) joint ;
    return true ;
}

static bool readRelativeRotation(osgDB::InputStream& is, osgODE::SliderJoint& joint)
{
    osg::Quat   q ;
    is >> q ;
    joint.setRelativeRotation(q) ;
    return true ;
}

static bool writeRelativeRotation(osgDB::OutputStream& os, const osgODE::SliderJoint& joint)
{
    osg::Quat   q = joint.getRelativeRotation() ;
    os << q << std::endl ;
    return true ;
}



static bool checkRelativePosition(const osgODE::SliderJoint& joint)
{
    (void) joint ;
    return true ;
}

static bool readRelativePosition(osgDB::InputStream& is, osgODE::SliderJoint& joint)
{
    osg::Vec3   v ;
    is >> v ;
    joint.setRelativePosition(v) ;
    return true ;
}

static bool writeRelativePosition(osgDB::OutputStream& os, const osgODE::SliderJoint& joint)
{
    osg::Vec3   v = joint.getRelativePosition() ;
    os << v << std::endl ;
    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( SliderJoint,
                         new osgODE::SliderJoint,
                         osgODE::SliderJoint,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::Joint osgODE::SliderJoint" )
{
    ADD_BOOL_SERIALIZER( AutoComputeRelativeValues, true ) ;

    ADD_USER_SERIALIZER( RelativeRotation ) ;
    ADD_USER_SERIALIZER( RelativePosition ) ;
}
/* ....................................................................... */
/* ======================================================================= */
