/*!
 * @file RigidBody_serializer.cpp
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
#include <osgODE/RigidBody>

#include <osgDB/Registry>

#include "real_serializer"
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
#define ADD_NONREF_VEC3_FUNC(V3_PROP, DEF) \
static bool check##V3_PROP(const osgODE::RigidBody& body) { return body.get##V3_PROP() != DEF ; } \
static bool read##V3_PROP(osgDB::InputStream& is, osgODE::RigidBody& body) \
{ \
    osg::Vec3   v ; \
    is >> v ; \
    body.set##V3_PROP(v) ; \
    return true ; \
} \
static bool write##V3_PROP(osgDB::OutputStream& os, const osgODE::RigidBody& body) \
{ \
    osg::Vec3   v = body.get##V3_PROP() ; \
    if( v != DEF ) { \
        os << v << std::endl ; \
        return true ; \
    } \
    return false ; \
}

#define ADD_NONREF_QUAT_FUNC(QUAT_PROP, DEF) \
static bool check##QUAT_PROP(const osgODE::RigidBody& body) { return body.get##QUAT_PROP() != DEF ; } \
static bool read##QUAT_PROP(osgDB::InputStream& is, osgODE::RigidBody& body) \
{ \
    osg::Quat   q ; \
    is >> q ; \
    body.set##QUAT_PROP(q) ; \
    return true ; \
} \
static bool write##QUAT_PROP(osgDB::OutputStream& os, const osgODE::RigidBody& body) \
{ \
    osg::Quat   q = body.get##QUAT_PROP() ; \
    if( q != DEF ) { \
        os << q << std::endl ; \
        return true ; \
    } \
    return false ; \
}

ADD_NONREF_VEC3_FUNC(Position, osg::Vec3()) ;
ADD_NONREF_QUAT_FUNC(Quaternion, osg::Quat()) ;
ADD_NONREF_VEC3_FUNC(LinearVelocity, osg::Vec3()) ;
ADD_NONREF_VEC3_FUNC(AngularVelocity, osg::Vec3()) ;
ADD_NONREF_VEC3_FUNC(Force, osg::Vec3()) ;
ADD_NONREF_VEC3_FUNC(Torque, osg::Vec3()) ;
ADD_NONREF_VEC3_FUNC(FiniteRotationAxis, osg::Vec3()) ;




static bool checkODEMass(const osgODE::RigidBody& body)
{
	(void) body ;

    return true ;
}

static bool readODEMass(osgDB::InputStream& is, osgODE::RigidBody& body)
{
    dMass   mass ;

    ooReal  tmp ;

    is >> tmp ; mass.mass = tmp ;

    is >> tmp ; mass.c[0] = tmp ;
    is >> tmp ; mass.c[1] = tmp ;
    is >> tmp ; mass.c[2] = tmp ;

    is >> tmp ; mass.I[0] = tmp ;
    is >> tmp ; mass.I[1] = tmp ;
    is >> tmp ; mass.I[2] = tmp ;
    is >> tmp ; mass.I[3] = tmp ;
    is >> tmp ; mass.I[4] = tmp ;
    is >> tmp ; mass.I[5] = tmp ;
    is >> tmp ; mass.I[6] = tmp ;
    is >> tmp ; mass.I[7] = tmp ;
    is >> tmp ; mass.I[8] = tmp ;
    is >> tmp ; mass.I[9] = tmp ;
    is >> tmp ; mass.I[10] = tmp ;
    is >> tmp ; mass.I[11] = tmp ;

    body.setODEMass(mass) ;

    return true ;
}

static bool writeODEMass(osgDB::OutputStream& os, const osgODE::RigidBody& body)
{
    dMass   mass = body.getODEMass() ;

    ooReal  tmp ;

    tmp = mass.mass ; os << tmp ;

    tmp = mass.c[0] ; os << tmp ;
    tmp = mass.c[1] ; os << tmp ;
    tmp = mass.c[2] ; os << tmp ;

    tmp = mass.I[0] ; os << tmp ;
    tmp = mass.I[1] ; os << tmp ;
    tmp = mass.I[2] ; os << tmp ;
    tmp = mass.I[3] ; os << tmp ;
    tmp = mass.I[4] ; os << tmp ;
    tmp = mass.I[5] ; os << tmp ;
    tmp = mass.I[6] ; os << tmp ;
    tmp = mass.I[7] ; os << tmp ;
    tmp = mass.I[8] ; os << tmp ;
    tmp = mass.I[9] ; os << tmp ;
    tmp = mass.I[10] ; os << tmp ;
    tmp = mass.I[11] ; os << tmp ;

    os << std::endl ;

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( RigidBody,
                         new osgODE::RigidBody,
                         osgODE::RigidBody,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::RigidBody" )
{
    ADD_USER_SERIALIZER(Position) ;
    ADD_USER_SERIALIZER(Quaternion) ;
    ADD_USER_SERIALIZER(LinearVelocity) ;
    ADD_USER_SERIALIZER(AngularVelocity) ;
    ADD_USER_SERIALIZER(Force) ;
    ADD_USER_SERIALIZER(Torque) ;
    ADD_BOOL_SERIALIZER(FiniteRotationMode, false) ;
    ADD_USER_SERIALIZER(FiniteRotationAxis) ;
    ADD_BOOL_SERIALIZER(GravityMode, true) ;

    ADD_BOOL_SERIALIZER(AutoDisableFlag, false) ;
    ADD_REAL_SERIALIZER(AutoDisableLinearThreshold, 0.1) ;
    ADD_REAL_SERIALIZER(AutoDisableAngularThreshold, 0.1) ;
    ADD_UINT_SERIALIZER(AutoDisableSteps, 180) ;
    ADD_REAL_SERIALIZER(AutoDisableTime, 3.0) ;
    ADD_UINT_SERIALIZER(AutoDisableAverageSamplesCount, 6) ;

    ADD_REAL_SERIALIZER(LinearDamping, 0.01) ;
    ADD_REAL_SERIALIZER(AngularDamping, 0.01) ;
    ADD_REAL_SERIALIZER(LinearDampingThreshold, 0.01) ;
    ADD_REAL_SERIALIZER(AngularDampingThreshold, 0.01) ;
    ADD_REAL_SERIALIZER(MaxAngularSpeed, FLT_MAX) ;

    ADD_REAL_SERIALIZER(Mass, 1.0) ;
    ADD_USER_SERIALIZER(ODEMass) ;

    ADD_BOOL_SERIALIZER(Kinematic, false) ;
    ADD_BOOL_SERIALIZER(BodyEnabled, true) ;
    ADD_BOOL_SERIALIZER(GyroscopicMode, true) ;
}
/* ....................................................................... */
/* ======================================================================= */
