/*!
 * @file DynamicParticleSystem_serializer.cpp
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
#include <osgODE/DynamicParticleSystem>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkBodyList(const osgODE::DynamicParticleSystem& dps)
{
    (void) dps ;

    return true ;
}

static bool writeBodyList(osgDB::OutputStream& os, const osgODE::DynamicParticleSystem& dps)
{
    const osgODE::DynamicParticleSystem::BodyList&    bodies = dps.getBodyList() ;

    os << (unsigned long int)bodies.size() << std::endl ;

    for(unsigned int i=0; i<bodies.size(); i++) {
        os << bodies[i].get() ;
    }

    return true ;
}

static bool readBodyList(osgDB::InputStream& is, osgODE::DynamicParticleSystem& dps)
{
    unsigned int    size = 0 ;
    is >> size ;

    if( size ) {
        osgODE::DynamicParticleSystem::BodyList bodies ;

        for(unsigned int i=0; i<size; i++) {

            osg::ref_ptr<osg::Object>   tmp = is.readObject() ;

            osgODE::RigidBody*  body = dynamic_cast<osgODE::RigidBody*>(tmp.get()) ;

            if( body ) {
                bodies.push_back( body ) ;
            }
        }

        dps.setBodyList( bodies ) ;
    }

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( DynamicParticleSystem,
                         new osgODE::DynamicParticleSystem,
                         osgODE::DynamicParticleSystem,
                         "osg::Object osg::Drawable osgParticle::ParticleSystem osgODE::DynamicParticleSystem" )
{
    ADD_OBJECT_SERIALIZER( World, osgODE::World, NULL ) ;
    ADD_OBJECT_SERIALIZER( BodyTemplate, osgODE::RigidBody, NULL ) ;
    ADD_USER_SERIALIZER( BodyList ) ;
}
/* ....................................................................... */
/* ======================================================================= */
