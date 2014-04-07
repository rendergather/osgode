/*!
 * @file Collidable_serializer.cpp
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
#include <osgODE/Collidable>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool
checkGeometrySize(const osgODE::Collidable& collidable)
{
    const osg::Vec3 size = collidable.getSize() ;

    return ! ( size.x() == 0.0  &&  size.y() == 0.0  &&  size.z() == 0.0 ) ;
}

static bool
readGeometrySize(osgDB::InputStream& is, osgODE::Collidable& collidable)
{
    osg::Vec3   size ;

    is >> size ;

    collidable.setSize(size) ;

    return true ;
}

static bool
writeGeometrySize(osgDB::OutputStream& os, const osgODE::Collidable& collidable)
{
    const osg::Vec3 size = collidable.getSize() ;

    os << size << std::endl ;

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Collidable,
                         /*new osgODE::Collidable*/ NULL,
                         osgODE::Collidable,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::RigidBody osgODE::Collidable" )
{
    ADD_UINT_SERIALIZER( CategoryBits, (unsigned int) -1 ) ;
    ADD_UINT_SERIALIZER( CollideBits, (unsigned int) -1 ) ;
    ADD_BOOL_SERIALIZER( GeometryEnabled, true ) ;
    ADD_USER_SERIALIZER( GeometrySize ) ;
    ADD_MATRIX_SERIALIZER( CollidableOffset, osg::Matrix::identity() ) ;
    ADD_OBJECT_SERIALIZER( CollisionParameters, osgODE::CollisionParameters, NULL ) ;
    ADD_OBJECT_SERIALIZER( CollisionCallback, osgODE::CollisionCallback, NULL ) ;
    ADD_UINT_SERIALIZER( LastCollisionFrame, 0 ) ;
}
/* ....................................................................... */
/* ======================================================================= */
