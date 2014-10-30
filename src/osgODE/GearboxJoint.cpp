/*!
 * @file GearboxJoint.cpp
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
#include <osgODE/GearboxJoint>
#include <osgODE/StaticWorld>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
GearboxJoint::GearboxJoint(void):
    m_ratio     ( 1.0 ),
    m_friction  ( 1.0 ),
    m_axis1     ( osg::Z_AXIS ),
    m_axis2     ( osg::Z_AXIS )
{
    this->setInfo(1, 1, 1) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
GearboxJoint::GearboxJoint(const GearboxJoint& other, const osg::CopyOp& copyop):
    BypassJoint ( other, copyop ),
    m_ratio     ( other.m_ratio ),
    m_friction  ( other.m_friction ),
    m_axis1     ( other.m_axis1 ),
    m_axis2     ( other.m_axis2 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
GearboxJoint::~GearboxJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
GearboxJoint::update( ooReal step_size )
{
    osg::Vec3   axis1 = m_axis1 ;
    osg::Vec3   axis2 = m_axis2 ;


    if( m_body1.valid() ) {
        axis1 = m_body1->getQuaternion() * m_axis1 ;
    }


    if( m_body2.valid() ) {
        axis2 = m_body2->getQuaternion() * m_axis2 ;
    }


    this->setRow( 0, axis1, osg::Vec3(), axis2 * m_ratio, osg::Vec3(), 0.0, -1.0 + 1.0 / m_friction ) ;


    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
GearboxJoint::setAxis1Implementation(const osg::Vec3& axis)
{
    if( m_body1.valid() ) {
        m_axis1 = m_body1->getQuaternion().inverse() * axis ;
    } else {
        m_axis1 = axis ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
GearboxJoint::readAxis1Implementation(osg::Vec3& axis)
{
    if( m_body1.valid() ) {
        axis = m_body1->getQuaternion() * m_axis1 ;
    } else {
        axis = m_axis1 ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
GearboxJoint::setAxis2Implementation(const osg::Vec3& axis)
{
    if( m_body2.valid() ) {
        m_axis2 = m_body2->getQuaternion().inverse() * axis ;
    } else {
        m_axis2 = axis ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
GearboxJoint::readAxis2Implementation(osg::Vec3& axis)
{
    if( m_body2.valid() ) {
        axis = m_body2->getQuaternion() * m_axis2 ;
    } else {
        axis = m_axis2 ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
