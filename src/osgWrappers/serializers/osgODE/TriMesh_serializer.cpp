/*!
 * @file TriMesh_serializer.cpp
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
#include <osgODE/TriMesh>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkVertexArray(const osgODE::TriMesh& trimesh)
{
    return trimesh.getVertexArray() != NULL ;
}

static bool writeVertexArray(osgDB::OutputStream& os, const osgODE::TriMesh& trimesh)
{
    const osg::Vec3Array*   arr = trimesh.getVertexArray() ;
    unsigned int            size = arr->size() ;

    if( arr ) {
        os << size << osgDB::BEGIN_BRACKET << std::endl ;

        for(unsigned int i=0; i<size; i++) {
            os << (*arr)[i] << std::endl ;
        }

        os << osgDB::END_BRACKET << std::endl ;
    }

    return true ;
}

static bool readVertexArray(osgDB::InputStream& is, osgODE::TriMesh& trimesh)
{
    unsigned int    size = 0 ;

    is >> size >> osgDB::BEGIN_BRACKET ;

    if( size != 0 ) {
        osg::Vec3Array* arr = new osg::Vec3Array() ;

        for(unsigned int i=0; i<size; i++) {

            osg::Vec3   v ;
            is >> v ;

            arr->push_back(v) ;

        }

        is >> osgDB::END_BRACKET ;

        trimesh.setVertexArray(arr) ;
    }

    return true ;
}


static bool checkIndexArray(const osgODE::TriMesh& trimesh)
{
    return trimesh.getIndexArray() != NULL ;
}

static bool writeIndexArray(osgDB::OutputStream& os, const osgODE::TriMesh& trimesh)
{
    const osg::IntArray*    arr = trimesh.getIndexArray() ;
    unsigned int            size = arr->size() ;

    if( arr ) {
        os << size << osgDB::BEGIN_BRACKET << std::endl ;

        for(unsigned int i=0; i<size; i+=3) {
            os << (*arr)[i] << (*arr)[i+1] << (*arr)[i+2] << std::endl ;
        }

        os << osgDB::END_BRACKET << std::endl ;
    }

    return true ;
}

static bool readIndexArray(osgDB::InputStream& is, osgODE::TriMesh& trimesh)
{
    unsigned int    size = 0 ;

    is >> size >> osgDB::BEGIN_BRACKET ;

    if( size != 0 ) {
        osg::IntArray*  arr = new osg::IntArray() ;

        for(unsigned int i=0; i<size; i++) {

            int s ;
            is >> s ;

            arr->push_back(s) ;

        }

        is >> osgDB::END_BRACKET ;

        trimesh.setIndexArray(arr) ;
    }

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( TriMesh,
                         new osgODE::TriMesh,
                         osgODE::TriMesh,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::RigidBody osgODE::Collidable osgODE::TriMesh" )
{
    ADD_USER_SERIALIZER(VertexArray) ;
    ADD_USER_SERIALIZER(IndexArray) ;






    struct BuildTriMesh: public osgDB::FinishedObjectReadCallback
    {
        virtual void    objectRead(osgDB::InputStream& is, osg::Object& obj)
        {
            (void) is ;
            static_cast<osgODE::TriMesh*>(&obj)->build() ;
        }
    } ;

    wrapper->addFinishedObjectReadCallback( new BuildTriMesh() ) ;
}
/* ....................................................................... */
/* ======================================================================= */
