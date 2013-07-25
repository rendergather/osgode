/*!
 * @file Engine_serializer.cpp
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
#include <osgODE/Engine>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkGearList(const osgODE::Engine& engine)
{
    return engine.getGearList().size() != 0 ;
}

static bool writeGearList(osgDB::OutputStream& os, const osgODE::Engine& engine)
{
    const osgODE::Engine::GearList&     gears = engine.getGearList() ;
    unsigned int                        size = gears.size() ;

    if( size > 0 ) {
        os << size << osgDB::BEGIN_BRACKET << std::endl ;

        for(unsigned int i=0; i<size; i++) {
            os << gears[i] << std::endl ;
        }

        os << osgDB::END_BRACKET << std::endl ;
    }

    return true ;
}

static bool readGearList(osgDB::InputStream& is, osgODE::Engine& engine)
{
    unsigned int    size = 0 ;

    is >> size >> osgDB::BEGIN_BRACKET ;

    if( size != 0 ) {
        osgODE::Engine::GearList      gears ;

        for(unsigned int i=0; i<size; i++) {

            double      v ;
            is >> v ;

            gears.push_back( v ) ;

        }

        is >> osgDB::END_BRACKET ;

        engine.setGearList(gears) ;
    }

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Engine,
                         new osgODE::Engine,
                         osgODE::Engine,
                         "osg::Object osgODE::EngineBase osgODE::Engine" )
{
    ADD_OBJECT_SERIALIZER( TorqueCurve, osgODE::ScalarInterpolator, NULL ) ;
    ADD_USER_SERIALIZER( GearList ) ;
    ADD_UINT_SERIALIZER( CurrentGear, 0 ) ;
}
/* ....................................................................... */
/* ======================================================================= */
