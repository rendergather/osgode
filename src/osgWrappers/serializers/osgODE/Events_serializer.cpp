/*!
 * @file Events_serializer.cpp
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
#include <osgODE/Events>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkKeyPressed(const osgODE::Events& events)
{
    (void) events ;

    for(unsigned int i=0; i<256; i++) {
        if( events.isKeyPressed(i) ) {
            return true ;
        }
    }

    return false ;
}

static bool writeKeyPressed(osgDB::OutputStream& os, const osgODE::Events& events)
{
    os << os.BEGIN_BRACKET << std::endl ;

    for(unsigned int i=0; i<256; i++) {
        os << events.isKeyPressed(i) << std::endl ;
    }

    os << os.END_BRACKET << std::endl ;

    return true ;
}

static bool readKeyPressed(osgDB::InputStream& is, osgODE::Events& events)
{
    is >> is.BEGIN_BRACKET ;


    for(unsigned int i=0; i<256; i++) {
        bool    pressed ;
        is >> pressed ;

        events.setKeyPressed( i, pressed ) ;
    }

    is >> is.END_BRACKET ;


    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Events,
                         new osgODE::Events,
                         osgODE::Events,
                         "osg::Object osgODE::Events" )
{
    ADD_VEC2_SERIALIZER( CursorPosition, osg::Vec2() ) ;
    ADD_VEC2_SERIALIZER( NormalizedCursorPosition, osg::Vec2() ) ;
    ADD_UINT_SERIALIZER( ButtonMask, 0 ) ;
    ADD_USER_SERIALIZER( KeyPressed ) ;
    ADD_OBJECT_SERIALIZER( View, osg::View, NULL ) ;
}
/* ....................................................................... */
/* ======================================================================= */
