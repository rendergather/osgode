/*!
 * @file RigidBodyServoMotor.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 by Rocco Martino                                   *
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
#include <osgODE/RigidBodyServoMotor>
#include <osgODE/RigidBody>
#include <osgODE/World>
#include <osgODE/Notify>

#include <osg/Math>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
RigidBodyServoMotor::RigidBodyServoMotor(void):
    m_mode          ( LINEAR ),
    m_max_force     ( 0.0 ),
    m_speed         ( 0.0 ),
    m_axis_local    ( true )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBodyServoMotor::RigidBodyServoMotor(const RigidBodyServoMotor& other, const osg::CopyOp& copyop):
    osgODE::ServoMotor      ( other, copyop ),
    m_mode                  ( other.m_mode ),
    m_axis                  ( other.m_axis ),
    m_max_force             ( other.m_max_force ),
    m_speed                 ( other.m_speed ),
    m_axis_local            ( other.m_axis_local)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBodyServoMotor::~RigidBodyServoMotor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBodyServoMotor::operator()( ODEObject* object )
{
    RigidBody*  body = object->asRigidBody() ;

    PS_ASSERT1( body != NULL ) ;


    switch( m_mode )
    {
        case LINEAR:    solveLinear( body ) ;   break ;
        case ANGULAR:   solveAngular( body ) ;  break ;

        default:        PS_BREAKPOINT() ;       break ;
    }


    traverse( object ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBodyServoMotor::solveLinear( RigidBody* body )
{
    osg::Vec3   direction = m_axis_local ? body->getQuaternion() * m_axis : m_axis ;

    World*      world = body->getWorld() ;
    PS_ASSERT1( world != NULL ) ;

    const double    err = m_speed - body->getLinearVelocity() * direction ;


    PIDController*  pid = getPIDController() ;

    double  force = 0.0 ;

    if( pid ) {
        force = pid->solve( err, world->getCurrentStepSize() ) ;
    } else {
        force = err ;
    }


    direction *= osg::sign( force ) ;


    force = osg::absolute( force ) ;


    if( m_max_force >= 0.0 ) {
        force = osg::minimum( force, m_max_force ) ;
    }

    body->addForce( direction * force ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBodyServoMotor::solveAngular( RigidBody* body )
{
    osg::Vec3   axis = m_axis_local ? body->getQuaternion() * m_axis : m_axis ;

    World*      world = body->getWorld() ;
    PS_ASSERT1( world != NULL ) ;

    const double    err = m_speed - body->getAngularVelocity() * axis ;


    PIDController*  pid = getPIDController() ;

    double  force = 0.0 ;

    if( pid ) {
        force = pid->solve( err, world->getCurrentStepSize() ) ;
    } else {
        force = err ;
    }


    axis *= osg::sign( force ) ;


    force = osg::absolute( force ) ;


    if( m_max_force >= 0.0 ) {
        force = osg::minimum( force, m_max_force ) ;
    }

    body->addTorque( axis * force ) ;
}
/* ....................................................................... */
/* ======================================================================= */
