/*!
 * @file MotionPath.cpp
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
#include <osgODE/MotionPath>
#include <osgODE/TargetJoint>
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
MotionPath::MotionPath(void):
    m_time(-1.0),
    m_status( STOP )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotionPath::MotionPath(const MotionPath& other, const osg::CopyOp& copyop):
    ODECallback(other, copyop),
    m_position( other.m_position ),
    m_quaternion( other.m_quaternion ),
    m_time( other.m_time ),
    m_status( other.m_status )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotionPath::~MotionPath(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotionPath::operator()(ODEObject* object)
{
    TargetJoint*    joint = object->asTargetJoint() ;

    PS_ASSERT1( joint != NULL ) ;


    switch( m_status )
    {
        case STOP:
            m_time = -1.0 ;
        break ;


        case PLAY:

            m_time = osg::maximum(m_time, 0.0) + joint->getWorld()->getCurrentStepSize() ;

            if( m_position ) {
                joint->setPosition( m_position->interpolate( m_time ) ) ;
                joint->setPositionTime( 0.0 ) ;
            }

            if( m_quaternion ) {
                joint->setQuaternion( m_quaternion->interpolate( m_time ) ) ;
                joint->setQuaternionTime( 0.0 ) ;
            }
        break ;


        case PAUSE:
        break ;
    } ;


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
