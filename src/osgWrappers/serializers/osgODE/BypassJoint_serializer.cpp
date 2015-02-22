/*!
 * @file BypassJoint_serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 - 2015 by Rocco Martino                            *
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
#if 0
namespace {
static bool checkRows(const osgODE::BypassJoint& j)
{
    (void) j ;
    return true ;
}

static bool writeRows(osgDB::OutputStream& os, const osgODE::BypassJoint& j)
{
    osg::Vec3   J1a, J1l, J2a, J2l ;
    ooReal      rhs, cfm ;

    os << os.BEGIN_BRACKET << std::endl ;

    for(unsigned int row=0; row<6; row++) {

        j.getRow(row, J1a, J1l, J2a, J2l, rhs, cfm) ;

        os << os.PROPERTY("Row") << row << os.BEGIN_BRACKET  << std::endl ;
        os << os.PROPERTY("J1a") << J1a << std::endl ;
        os << os.PROPERTY("J1l") << J1l << std::endl ;
        os << os.PROPERTY("J2a") << J2a << std::endl ;
        os << os.PROPERTY("J2l") << J2l << std::endl ;
        os << os.PROPERTY("rhs") << rhs << std::endl ;
        os << os.PROPERTY("cfm") << cfm << std::endl ;
        os << os.END_BRACKET << std::endl ;
    }

    os << os.END_BRACKET << std::endl ;

    return true ;
}

static bool readRows(osgDB::InputStream& is, osgODE::BypassJoint& j)
{
    osg::Vec3   J1a, J1l, J2a, J2l ;
    ooReal      rhs, cfm ;

    is >> is.BEGIN_BRACKET ;

    for(unsigned int row=0; row<6; row++) {

        is >> is.PROPERTY("Row") >> row >> is.BEGIN_BRACKET ;
        is >> is.PROPERTY("J1a") >> J1a ;
        is >> is.PROPERTY("J1l") >> J1l ;
        is >> is.PROPERTY("J2a") >> J2a ;
        is >> is.PROPERTY("J2l") >> J2l ;
        is >> is.PROPERTY("rhs") >> rhs ;
        is >> is.PROPERTY("cfm") >> cfm ;
        is >> is.END_BRACKET ;


        j.setRow(row, J1a, J1l, J2a, J2l, rhs, cfm) ;
    }

    is >> is.END_BRACKET ;


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

    os << os.BEGIN_BRACKET << std::endl ;

    j.getInfo(max_m, m, nub) ;

    os << os.PROPERTY("MaxM") << max_m << std::endl ;
    os << os.PROPERTY("M") << m << std::endl ;
    os << os.PROPERTY("Nub") << nub << std::endl ;

    os << os.END_BRACKET << std::endl ;

    return true ;
}

static bool readInfo(osgDB::InputStream& is, osgODE::BypassJoint& j)
{
    unsigned int    max_m, m, nub ;

    is >> is.BEGIN_BRACKET ;

    is >> is.PROPERTY("MaxM") >> max_m ;
    is >> is.PROPERTY("M") >> m ;
    is >> is.PROPERTY("Nub") >> nub ;

    is >> is.END_BRACKET ;



    j.setInfo(max_m, m, nub) ;


    return true ;
}
} // anon namespace
#endif
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( BypassJoint,
                         new osgODE::BypassJoint,
                         osgODE::BypassJoint,
                         "osg::Object osgODE::ODEObject osgODE::Transformable osgODE::Joint osgODE::BypassJoint" )
{
#if 0
    ADD_USER_SERIALIZER( Rows ) ;
    ADD_USER_SERIALIZER( Info ) ;
#endif

    ADD_MATRIX_SERIALIZER( InitialTransformation, osg::Matrix::identity() ) ;
}
/* ....................................................................... */
/* ======================================================================= */
