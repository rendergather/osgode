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
namespace {

static bool checkFootContactInfo( const osgODE::CharacterBase& obj )
{
    (void) obj ;
    return true ;
}



static bool writeFootContactInfo( osgDB::OutputStream& os, const osgODE::CharacterBase& obj )
{
    const osgODE::CharacterBase::FootContactInfo&   info = obj.getFootContactInfo() ;

    os << os.BEGIN_BRACKET << std::endl ;

        os << os.PROPERTY("Spring")     << info.Spring      << std::endl ;
        os << os.PROPERTY("Damper")     << info.Damper      << std::endl ;
        os << os.PROPERTY("MaxDepth")   << info.MaxDepth    << std::endl ;

    os << os.END_BRACKET << std::endl ;


    return true;
}



static bool readFootContactInfo( osgDB::InputStream& is, osgODE::CharacterBase& obj )
{
    osgODE::CharacterBase::FootContactInfo info ;

    is >> is.BEGIN_BRACKET ;

        is >> is.PROPERTY("Spring")     >> info.Spring ;
        is >> is.PROPERTY("Damper")     >> info.Damper ;
        is >> is.PROPERTY("MaxDepth")   >> info.MaxDepth ;

    is >> is.END_BRACKET ;


    obj.setFootContactInfo( info ) ;

    return true;
}










static bool checkFootstepInfo( const osgODE::CharacterBase& obj )
{
    (void) obj ;
    return true ;
}



static bool writeFootstepInfo( osgDB::OutputStream& os, const osgODE::CharacterBase& obj )
{
    const osgODE::CharacterBase::FootstepInfo&  info = obj.getFootstepInfo() ;

    os << os.BEGIN_BRACKET << std::endl ;

        os << os.PROPERTY("Magnitude")      << info.Magnitude       << std::endl ;
        os << os.PROPERTY("PowerFactor")    << info.PowerFactor     << std::endl ;
        os << os.PROPERTY("TimeMultiplier") << info.TimeMultiplier  << std::endl ;
        os << os.PROPERTY("SpeedThreshold") << info.SpeedThreshold  << std::endl ;

    os << os.END_BRACKET << std::endl ;


    return true;
}



static bool readFootstepInfo( osgDB::InputStream& is, osgODE::CharacterBase& obj )
{
    osgODE::CharacterBase::FootstepInfo info ;

    is >> is.BEGIN_BRACKET ;

        is >> is.PROPERTY("Magnitude")      >> info.Magnitude ;
        is >> is.PROPERTY("PowerFactor")    >> info.PowerFactor ;
        is >> is.PROPERTY("TimeMultiplier") >> info.TimeMultiplier ;
        is >> is.PROPERTY("SpeedThreshold") >> info.SpeedThreshold ;

    is >> is.END_BRACKET ;


    obj.setFootstepInfo( info ) ;

    return true;
}



} // anon namespace
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
    ADD_DOUBLE_SERIALIZER( Elasticity, 0.067 ) ;
    ADD_DOUBLE_SERIALIZER( Height, 1.75 ) ;
    ADD_USER_SERIALIZER( FootContactInfo ) ;
    ADD_USER_SERIALIZER( FootstepInfo ) ;
}
/* ....................................................................... */
/* ======================================================================= */
