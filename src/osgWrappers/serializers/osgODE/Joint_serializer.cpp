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



#define ADD_ALL_PAR_FUNC( X ) \
    ADD_PAR_FUNC(dParamLoStop##X, -FLT_MAX) ; \
    ADD_PAR_FUNC(dParamHiStop##X, FLT_MAX) ; \
    ADD_PAR_FUNC(dParamVel##X, 0.0) ; \
    ADD_PAR_FUNC(dParamLoVel##X, FLT_MAX) ; \
    ADD_PAR_FUNC(dParamHiVel##X, FLT_MAX) ; \
    ADD_PAR_FUNC(dParamFMax##X, 0) ; \
    ADD_PAR_FUNC(dParamFudgeFactor##X, 0.0) ; \
    ADD_PAR_FUNC(dParamBounce##X, 0.0) ; \
    ADD_PAR_FUNC(dParamCFM##X, 0.0) ; \
    ADD_PAR_FUNC(dParamStopERP##X, 1.0) ; \
    ADD_PAR_FUNC(dParamStopCFM##X, 0.0) ; \
    ADD_PAR_FUNC(dParamSuspensionERP##X, 1.0) ; \
    ADD_PAR_FUNC(dParamSuspensionCFM##X, 0.0) ; \
    ADD_PAR_FUNC(dParamERP##X, 1.0) ;



ADD_ALL_PAR_FUNC(1) ;
ADD_ALL_PAR_FUNC(2) ;
ADD_ALL_PAR_FUNC(3) ;
ADD_ALL_PAR_FUNC(4) ;
ADD_ALL_PAR_FUNC(5) ;
ADD_ALL_PAR_FUNC(6) ;
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


#define ADD_ALL_PAR_SERIALIZERS( X ) \
    ADD_USER_SERIALIZER(dParamLoStop##X) ; \
    ADD_USER_SERIALIZER(dParamHiStop##X) ; \
    ADD_USER_SERIALIZER(dParamVel##X) ; \
    ADD_USER_SERIALIZER(dParamLoVel##X) ; \
    ADD_USER_SERIALIZER(dParamHiVel##X) ; \
    ADD_USER_SERIALIZER(dParamFMax##X) ; \
    ADD_USER_SERIALIZER(dParamFudgeFactor##X) ; \
    ADD_USER_SERIALIZER(dParamBounce##X) ; \
    ADD_USER_SERIALIZER(dParamCFM##X) ; \
    ADD_USER_SERIALIZER(dParamStopERP##X) ; \
    ADD_USER_SERIALIZER(dParamStopCFM##X) ; \
    ADD_USER_SERIALIZER(dParamSuspensionERP##X) ; \
    ADD_USER_SERIALIZER(dParamSuspensionCFM##X) ; \
    ADD_USER_SERIALIZER(dParamERP##X) ;

    ADD_ALL_PAR_SERIALIZERS(1) ;
    ADD_ALL_PAR_SERIALIZERS(2) ;
    ADD_ALL_PAR_SERIALIZERS(3) ;
    ADD_ALL_PAR_SERIALIZERS(4) ;
    ADD_ALL_PAR_SERIALIZERS(5) ;
    ADD_ALL_PAR_SERIALIZERS(6) ;


    ADD_BOOL_SERIALIZER( JointEnabled, true ) ;
}
/* ....................................................................... */
/* ======================================================================= */
