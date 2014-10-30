/*!
 * @file Vec3Interpolator_serializer.cpp
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
#include <osgODE/Interpolator>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkPointList(const osgODE::Vec3Interpolator& interpolator)
{
    return interpolator.getPointList().size() != 0 ;
}

static bool writePointList(osgDB::OutputStream& os, const osgODE::Vec3Interpolator& interpolator)
{
    const osgODE::Vec3Interpolator::PointList&  points = interpolator.getPointList() ;
    unsigned int            size = points.size() ;

    if( size > 0 ) {
        os << size << os.BEGIN_BRACKET << std::endl ;

        for(unsigned int i=0; i<size; i++) {
            os << points[i].first << points[i].second << std::endl ;
        }

        os << os.END_BRACKET << std::endl ;
    }

    return true ;
}

static bool readPointList(osgDB::InputStream& is, osgODE::Vec3Interpolator& interpolator)
{
    unsigned int    size = 0 ;

    is >> size >> is.BEGIN_BRACKET ;

    if( size != 0 ) {
        osgODE::Vec3Interpolator::PointList points ;

        for(unsigned int i=0; i<size; i++) {

            ooReal      t ;
            osg::Vec3   v ;
            is >> t ;
            is >> v ;

            points.push_back( osgODE::Vec3Interpolator::Point(t, v) ) ;

        }

        is >> is.END_BRACKET ;

        interpolator.setPointList(points) ;
    }

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Vec3Interpolator,
                         NULL /*new osgODE::Vec3Interpolator*/,
                         osgODE::Vec3Interpolator,
                         "osg::Object osgODE::InterpolatorBase osgODE::Vec3Interpolator" )
{
    ADD_USER_SERIALIZER(PointList) ;
}
/* ....................................................................... */
/* ======================================================================= */
