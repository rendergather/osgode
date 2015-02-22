/*!
 * @file Plane2DJoint.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Plane2DJoint>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Plane2DJoint::Plane2DJoint(void):
    m_normal    ( osg::Z_AXIS )
{
    setAxis1( m_normal ) ;

    this->setSureMaxInfo( 1 ) ;
    this->setInfo1( 1, 1 ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Plane2DJoint::Plane2DJoint(const Plane2DJoint& other, const osg::CopyOp& copyop):
    BypassJoint     ( other, copyop ),
    m_normal        ( other.m_normal ),
    m_origin        ( other.m_origin )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Plane2DJoint::~Plane2DJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Plane2DJoint::update( ooReal step_size )
{
    ooReal      rhs = 0.0 ;
    ooReal      cfm = m_cfm[0] ;


    if( m_body1.valid() ) {
        osg::Vec3   v = m_origin - m_body1->getPosition() ;
        rhs = (v * m_normal) * m_erp[0] / step_size ;
    }

    this->setRow( 0, osg::Vec3(), m_normal, osg::Vec3(), osg::Vec3(), rhs, cfm, -dInfinity, dInfinity ) ;


    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Plane2DJoint::setAxis1Implementation(const osg::Vec3& axis)
{
    m_normal = axis ;
    m_normal.normalize() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Plane2DJoint::readAxis1Implementation(osg::Vec3& axis)
{
    axis.set( m_normal ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Plane2DJoint::setAnchor1Implementation(const osg::Vec3& anchor)
{
    m_origin = anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Plane2DJoint::readAnchor1Implementation(osg::Vec3& anchor)
{
    anchor.set( m_origin ) ;
}
/* ....................................................................... */
/* ======================================================================= */
