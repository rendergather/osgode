/*!
 * @file AerodynamicDevice_serializer.cpp
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
#include <osgODE/AerodynamicDevice>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkDragPointList(const osgODE::AerodynamicDevice& ad)
{
    (void) ad ;
    return true ;
}

static bool writeDragPointList(osgDB::OutputStream& os, const osgODE::AerodynamicDevice& ad)
{
    const osgODE::AerodynamicDevice::DragPointList& points = ad.getDragPointList() ;
    unsigned int            size = points.size() ;

    os << size << osgDB::BEGIN_BRACKET << std::endl ;

    for(unsigned int i=0; i<size; i++) {
        os << points[i] << std::endl ;
    }

    os << osgDB::END_BRACKET << std::endl ;

    return true ;
}

static bool readDragPointList(osgDB::InputStream& is, osgODE::AerodynamicDevice& ad)
{
    unsigned int    size = 0 ;

    is >> size >> osgDB::BEGIN_BRACKET ;

    osgODE::AerodynamicDevice::DragPointList    points ;

    for(unsigned int i=0; i<size; i++) {

        osg::Vec4   v ;
        is >> v ;

        points.push_back(v) ;

    }

    is >> osgDB::END_BRACKET ;


    ad.setDragPointList(points) ;


    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( AerodynamicDevice,
                         new osgODE::AerodynamicDevice,
                         osgODE::AerodynamicDevice,
                         "osg::Object osgODE::ODECallback osgODE::AerodynamicDevice" )
{
    ADD_USER_SERIALIZER( DragPointList ) ;
}
/* ....................................................................... */
/* ======================================================================= */
