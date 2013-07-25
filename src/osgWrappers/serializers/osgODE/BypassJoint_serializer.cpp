/*!
 * @file BypassJoint_serializer.cpp
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
#include <osgODE/BypassJoint>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkRows(const osgODE::BypassJoint& j)
{
    (void) j ;
    return true ;
}

static bool writeRows(osgDB::OutputStream& os, const osgODE::BypassJoint& j)
{
    osg::Vec3   J1a, J1l, J2a, J2l ;
    double      rhs, cfm ;

    os << osgDB::BEGIN_BRACKET << std::endl ;

    for(unsigned int row=0; row<6; row++) {

        j.getRow(row, J1a, J1l, J2a, J2l, rhs, cfm) ;

        os << osgDB::PROPERTY("Row") << row << osgDB::BEGIN_BRACKET  << std::endl ;
        os << osgDB::PROPERTY("J1a") << J1a << std::endl ;
        os << osgDB::PROPERTY("J1l") << J1l << std::endl ;
        os << osgDB::PROPERTY("J2a") << J2a << std::endl ;
        os << osgDB::PROPERTY("J2l") << J2l << std::endl ;
        os << osgDB::PROPERTY("rhs") << rhs << std::endl ;
        os << osgDB::PROPERTY("cfm") << cfm << std::endl ;
        os << osgDB::END_BRACKET << std::endl ;
    }

    os << osgDB::END_BRACKET << std::endl ;

    return true ;
}

static bool readRows(osgDB::InputStream& is, osgODE::BypassJoint& j)
{
    osg::Vec3   J1a, J1l, J2a, J2l ;
    double      rhs, cfm ;

    is >> osgDB::BEGIN_BRACKET ;

    for(unsigned int row=0; row<6; row++) {

        is >> osgDB::PROPERTY("Row") >> row >> osgDB::BEGIN_BRACKET ;
        is >> osgDB::PROPERTY("J1a") >> J1a ;
        is >> osgDB::PROPERTY("J1l") >> J1l ;
        is >> osgDB::PROPERTY("J2a") >> J2a ;
        is >> osgDB::PROPERTY("J2l") >> J2l ;
        is >> osgDB::PROPERTY("rhs") >> rhs ;
        is >> osgDB::PROPERTY("cfm") >> cfm ;
        is >> osgDB::END_BRACKET ;


        j.setRow(row, J1a, J1l, J2a, J2l, rhs, cfm) ;
    }

    is >> osgDB::END_BRACKET ;


    return true ;
}





static bool checkInfo(const osgODE::BypassJoint& j)
{
    (void) j ;
    return true ;
}

static bool writeInfo(osgDB::OutputStream& os, const osgODE::BypassJoint& j)
{
    unsigned int    max_m, m, nub ;

    os << osgDB::BEGIN_BRACKET << std::endl ;

    j.getInfo(max_m, m, nub) ;

    os << osgDB::PROPERTY("MaxM") << max_m << std::endl ;
    os << osgDB::PROPERTY("M") << m << std::endl ;
    os << osgDB::PROPERTY("Nub") << nub << std::endl ;

    os << osgDB::END_BRACKET << std::endl ;

    return true ;
}

static bool readInfo(osgDB::InputStream& is, osgODE::BypassJoint& j)
{
    unsigned int    max_m, m, nub ;

    is >> osgDB::BEGIN_BRACKET ;

    is >> osgDB::PROPERTY("MaxM") >> max_m ;
    is >> osgDB::PROPERTY("M") >> m ;
    is >> osgDB::PROPERTY("Nub") >> nub ;

    is >> osgDB::END_BRACKET ;



    j.setInfo(max_m, m, nub) ;


    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( BypassJoint,
                         new osgODE::BypassJoint,
                         osgODE::BypassJoint,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::Joint osgODE::BypassJoint" )
{
    ADD_USER_SERIALIZER( Rows ) ;
    ADD_USER_SERIALIZER( Info ) ;
}
/* ....................................................................... */
/* ======================================================================= */
