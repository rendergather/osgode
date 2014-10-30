/*!
 * @file Joint_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2013 by Rocco Martino                            *
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
#include <osgODE/Joint>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool
checkAnchor1(const osgODE::Joint& joint)
{
    return const_cast<osgODE::Joint&>(joint).getAnchor1() != osg::Vec3(0.0, 0.0, 0.0) ;
}

static bool
writeAnchor1(osgDB::OutputStream& os, const osgODE::Joint& joint)
{
    os << const_cast<osgODE::Joint&>(joint).getAnchor1() << std::endl ;
    return true ;
}

static bool
readAnchor1(osgDB::InputStream& is, osgODE::Joint& joint)
{
    osg::Vec3   v ;
    is >> v ;
    joint.setAnchor1(v) ;
    return true ;
}



static bool
checkAnchor2(const osgODE::Joint& joint)
{
    return const_cast<osgODE::Joint&>(joint).getAnchor2() != osg::Vec3(0.0, 0.0, 0.0) ;
}

static bool
writeAnchor2(osgDB::OutputStream& os, const osgODE::Joint& joint)
{
    os << const_cast<osgODE::Joint&>(joint).getAnchor2() << std::endl ;
    return true ;
}

static bool
readAnchor2(osgDB::InputStream& is, osgODE::Joint& joint)
{
    osg::Vec3   v ;
    is >> v ;
    joint.setAnchor2(v) ;
    return true ;
}


#define ADD_AXIS_FUNC(AXIS_PROP, DEF_VAL) \
static bool check##AXIS_PROP(const osgODE::Joint& joint) { return const_cast<osgODE::Joint&>(joint).get##AXIS_PROP() != (DEF_VAL) ; } \
static bool write##AXIS_PROP(osgDB::OutputStream& os, const osgODE::Joint& joint) \
{ \
    os << const_cast<osgODE::Joint&>(joint).get##AXIS_PROP() << std::endl ; \
    return true ; \
} \
static bool read##AXIS_PROP(osgDB::InputStream& is, osgODE::Joint& joint) \
{ \
    osg::Vec3   v ; \
    is >> v ; \
    joint.set##AXIS_PROP(v) ; \
    return true ; \
} \


ADD_AXIS_FUNC(Axis1, osg::X_AXIS) ;
ADD_AXIS_FUNC(Axis2, osg::Y_AXIS) ;
ADD_AXIS_FUNC(Axis3, osg::Z_AXIS) ;


#define ADD_PAR_FUNC(PAR_PROP, DEF_VAL) \
static bool check##PAR_PROP(const osgODE::Joint& joint) { return const_cast<osgODE::Joint&>(joint).getParam(PAR_PROP) != (DEF_VAL) ; } \
static bool write##PAR_PROP(osgDB::OutputStream& os, const osgODE::Joint& joint) \
{ \
    ooReal  par = const_cast<osgODE::Joint&>(joint).getParam(PAR_PROP) ; \
    if( par >= dInfinity )           par = FLT_MAX ; \
    else if ( par <= -dInfinity )    par = -FLT_MAX ; \
    os << par << std::endl ; \
    return true ; \
} \
static bool read##PAR_PROP(osgDB::InputStream& is, osgODE::Joint& joint) \
{ \
    ooReal  par ; \
    is >> par ; \
    joint.setParam(PAR_PROP, par) ; \
    return true ; \
}

ADD_PAR_FUNC(dParamLoStop, FLT_MAX) ;
ADD_PAR_FUNC(dParamHiStop, -FLT_MAX) ;
ADD_PAR_FUNC(dParamVel, 0.0) ;
ADD_PAR_FUNC(dParamFMax, 0) ;
ADD_PAR_FUNC(dParamFudgeFactor, 0.0) ;
ADD_PAR_FUNC(dParamBounce, 0.0) ;
ADD_PAR_FUNC(dParamCFM, 0.0) ;
ADD_PAR_FUNC(dParamStopERP, 1.0) ;
ADD_PAR_FUNC(dParamStopCFM, 0.0) ;
ADD_PAR_FUNC(dParamSuspensionERP, 1.0) ;
ADD_PAR_FUNC(dParamSuspensionCFM, 0.0) ;
ADD_PAR_FUNC(dParamERP, 1.0) ;

ADD_PAR_FUNC(dParamLoStop2, FLT_MAX) ;
ADD_PAR_FUNC(dParamHiStop2, -FLT_MAX) ;
ADD_PAR_FUNC(dParamVel2, 0.0) ;
ADD_PAR_FUNC(dParamFMax2, 0) ;
ADD_PAR_FUNC(dParamFudgeFactor2, 0.0) ;
ADD_PAR_FUNC(dParamBounce2, 0.0) ;
ADD_PAR_FUNC(dParamCFM2, 0.0) ;
ADD_PAR_FUNC(dParamStopERP2, 1.0) ;
ADD_PAR_FUNC(dParamStopCFM2, 0.0) ;
ADD_PAR_FUNC(dParamSuspensionERP2, 1.0) ;
ADD_PAR_FUNC(dParamSuspensionCFM2, 0.0) ;
ADD_PAR_FUNC(dParamERP2, 1.0) ;

ADD_PAR_FUNC(dParamLoStop3, FLT_MAX) ;
ADD_PAR_FUNC(dParamHiStop3, -FLT_MAX) ;
ADD_PAR_FUNC(dParamVel3, 0.0) ;
ADD_PAR_FUNC(dParamFMax3, 0) ;
ADD_PAR_FUNC(dParamFudgeFactor3, 0.0) ;
ADD_PAR_FUNC(dParamBounce3, 0.0) ;
ADD_PAR_FUNC(dParamCFM3, 0.0) ;
ADD_PAR_FUNC(dParamStopERP3, 1.0) ;
ADD_PAR_FUNC(dParamStopCFM3, 0.0) ;
ADD_PAR_FUNC(dParamSuspensionERP3, 1.0) ;
ADD_PAR_FUNC(dParamSuspensionCFM3, 0.0) ;
ADD_PAR_FUNC(dParamERP3, 1.0) ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Joint,
                         /*new osgODE::Joint*/ NULL,
                         osgODE::Joint,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::Joint" )
{
    ADD_OBJECT_SERIALIZER( Body1, osgODE::RigidBody, NULL ) ;
    ADD_OBJECT_SERIALIZER( Body2, osgODE::RigidBody, NULL ) ;

    ADD_USER_SERIALIZER( Anchor1 ) ;
    ADD_USER_SERIALIZER( Anchor2 ) ;
    ADD_USER_SERIALIZER( Axis1 ) ;
    ADD_USER_SERIALIZER( Axis2 ) ;
    ADD_USER_SERIALIZER( Axis3 ) ;

    ADD_USER_SERIALIZER(dParamLoStop) ;
    ADD_USER_SERIALIZER(dParamHiStop) ;
    ADD_USER_SERIALIZER(dParamVel) ;
    ADD_USER_SERIALIZER(dParamFMax) ;
    ADD_USER_SERIALIZER(dParamFudgeFactor) ;
    ADD_USER_SERIALIZER(dParamBounce) ;
    ADD_USER_SERIALIZER(dParamCFM) ;
    ADD_USER_SERIALIZER(dParamStopERP) ;
    ADD_USER_SERIALIZER(dParamStopCFM) ;
    ADD_USER_SERIALIZER(dParamSuspensionERP) ;
    ADD_USER_SERIALIZER(dParamSuspensionCFM) ;
    ADD_USER_SERIALIZER(dParamERP) ;

    ADD_USER_SERIALIZER(dParamLoStop2) ;
    ADD_USER_SERIALIZER(dParamHiStop2) ;
    ADD_USER_SERIALIZER(dParamVel2) ;
    ADD_USER_SERIALIZER(dParamFMax2) ;
    ADD_USER_SERIALIZER(dParamFudgeFactor2) ;
    ADD_USER_SERIALIZER(dParamBounce2) ;
    ADD_USER_SERIALIZER(dParamCFM2) ;
    ADD_USER_SERIALIZER(dParamStopERP2) ;
    ADD_USER_SERIALIZER(dParamStopCFM2) ;
    ADD_USER_SERIALIZER(dParamSuspensionERP2) ;
    ADD_USER_SERIALIZER(dParamSuspensionCFM2) ;
    ADD_USER_SERIALIZER(dParamERP2) ;

    ADD_USER_SERIALIZER(dParamLoStop3) ;
    ADD_USER_SERIALIZER(dParamHiStop3) ;
    ADD_USER_SERIALIZER(dParamVel3) ;
    ADD_USER_SERIALIZER(dParamFMax3) ;
    ADD_USER_SERIALIZER(dParamFudgeFactor3) ;
    ADD_USER_SERIALIZER(dParamBounce3) ;
    ADD_USER_SERIALIZER(dParamCFM3) ;
    ADD_USER_SERIALIZER(dParamStopERP3) ;
    ADD_USER_SERIALIZER(dParamStopCFM3) ;
    ADD_USER_SERIALIZER(dParamSuspensionERP3) ;
    ADD_USER_SERIALIZER(dParamSuspensionCFM3) ;
    ADD_USER_SERIALIZER(dParamERP3) ;


    ADD_BOOL_SERIALIZER( JointEnabled, true ) ;
}
/* ....................................................................... */
/* ======================================================================= */
