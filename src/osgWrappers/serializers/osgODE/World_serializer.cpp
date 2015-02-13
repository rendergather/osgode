/*!
 * @file World_serializer.cpp
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
#include <osgODE/World>

#include <osgDB/Registry>

#include "real_serializer"
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkGravity(const osgODE::World& world)
{
    return world.getGravity() != osg::Vec3(0, 0, -9.80665) ;
}

static bool writeGravity(osgDB::OutputStream& os, const osgODE::World& world)
{
    if( world.getGravity() != osg::Vec3(0, 0, -9.80665) ) {
        os << world.getGravity() << std::endl ;
        return true ;
    }
    return false ;
}

static bool readGravity(osgDB::InputStream& is, osgODE::World& world)
{
    osg::Vec3   g ;
    is >> g ;
    world.setGravity(g) ;
    return true ;
}


static bool checkWorldStepFunction(const osgODE::World& world)
{
    (void) world ;

    return true ;
}

static bool writeWorldStepFunction(osgDB::OutputStream& os, const osgODE::World& world)
{
    osgODE::World::WorldStepPrototype   ws = world.getWorldStepFunction() ;

    if( ws == dWorldQuickStep ) {
        os << "dWorldQuickStep" << std::endl ;

    } else {
        os << "dWorldStep" << std::endl ;
    }

    return true ;
}

static bool readWorldStepFunction(osgDB::InputStream& is, osgODE::World& world)
{
    std::string ws ;
    is >> ws ;

    if( ws == "dWorldQuickStep" ) {
        world.setWorldStepFunction(dWorldQuickStep) ;

    } else {
        world.setWorldStepFunction(dWorldStep) ;
    }

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( World,
                         new osgODE::World,
                         osgODE::World,
                         "osg::Object osgODE::ODEObject osgODE::World" )
{
    ADD_UINT_SERIALIZER(CurrentFrame, 0) ;
    ADD_REAL_SERIALIZER(SimulationTime, 0.0) ;
    ADD_USER_SERIALIZER(Gravity) ;
    ADD_VEC3_SERIALIZER(Wind, osg::Vec3()) ;
    ADD_REAL_SERIALIZER(WindFrequency, 0.0) ;
    ADD_REAL_SERIALIZER(AirDensity, 1.2929) ;

    ADD_REAL_SERIALIZER(ERP, 0.2) ;
    ADD_REAL_SERIALIZER(CFM, 1.0e-5) ;

    ADD_INT_SERIALIZER(QuickStepNumIterations, 20) ;
    ADD_REAL_SERIALIZER(QuickStepW, 1.3) ;
    ADD_REAL_SERIALIZER(ContactMaxCorrectingVel, FLT_MAX) ;
    ADD_REAL_SERIALIZER(ContactSurfaceLayer, 0.0) ;

    ADD_USER_SERIALIZER(WorldStepFunction) ;

    ADD_LIST_SERIALIZER(ObjectList, osgODE::World::ObjectList) ;

    ADD_OBJECT_SERIALIZER( FrontEventsBuffer, osgODE::Events, NULL ) ;
    ADD_OBJECT_SERIALIZER( BackEventsBuffer, osgODE::Events, NULL ) ;
}
/* ....................................................................... */
/* ======================================================================= */
