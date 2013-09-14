/*!
 * @file UniversalServoMotor.cpp
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
#include <osgODE/UniversalServoMotor>
#include <osgODE/UniversalJoint>
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
UniversalServoMotor::UniversalServoMotor(void)
{
    m_position[0] = 0.0 ;
    m_position[1] = 0.0 ;

    m_force[0] = 0.0 ;
    m_force[1] = 0.0 ;

    m_gain[0] = 1.0 ;
    m_gain[1] = 1.0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
UniversalServoMotor::UniversalServoMotor(const UniversalServoMotor& other, const osg::CopyOp& copyop):
    ODECallback     ( other,  copyop)
{
    m_position[0] = other.m_position[0] ;
    m_position[1] = other.m_position[1] ;

    m_force[0] = other.m_force[0] ;
    m_force[1] = other.m_force[1] ;

    m_gain[0] = other.m_gain[0] ;
    m_gain[1] = other.m_gain[1] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
UniversalServoMotor::~UniversalServoMotor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
UniversalServoMotor::operator()(ODEObject* object)
{

    UniversalJoint*     universal = object->asUniversalJoint() ;
    PS_ASSERT1( universal != NULL ) ;

    World*      world = object->getWorld() ;
    PS_ASSERT1( world != NULL ) ;


    const double    step_size = world->getCurrentStepSize() ;

    const double    err1 = m_position[0] - universal->getAngle1() ;
    const double    err2 = m_position[1] - universal->getAngle2() ;

    const double    gain1 = m_gain[0] / step_size ;
    const double    gain2 = m_gain[1] / step_size ;

    const double    vel1 = gain1 * err1 ;
    const double    vel2 = gain2 * err2 ;

    universal->setParam( dParamFMax1, m_force[0] ) ;
    universal->setParam( dParamVel1, vel1 ) ;

    universal->setParam( dParamFMax2, m_force[1] ) ;
    universal->setParam( dParamVel2, vel2 ) ;


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
UniversalServoMotor*
UniversalServoMotor::asUniversalServoMotor(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
