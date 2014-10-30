/*!
 * @file SliderServoMotor.cpp
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/SliderServoMotor>
#include <osgODE/SliderJoint>
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
SliderServoMotor::SliderServoMotor(void):
    m_position      ( 0.0 ),
    m_force         ( 0.0 ),
    m_max_vel       ( -1.0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SliderServoMotor::SliderServoMotor(const SliderServoMotor& other, const osg::CopyOp& copyop):
    ServoMotor      ( other,  copyop),
    m_position      ( other.m_position ),
    m_force         ( other.m_force ),
    m_max_vel       ( other.m_max_vel )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SliderServoMotor::~SliderServoMotor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
SliderServoMotor::operator()(ODEObject* object)
{

    SliderJoint*     slider = object->asSliderJoint() ;
    PS_ASSERT1( slider != NULL ) ;

    World*      world = object->getWorld() ;
    PS_ASSERT1( world != NULL ) ;


    PIDController*  pid = getPIDController() ;

    ooReal  vel = 0.0 ;

    if( pid ) {
        vel = pid->solve( m_position - slider->getPosition(), world->getCurrentStepSize() ) ;
    } else {
        vel = m_position - slider->getPosition() ;
    }



    if( m_max_vel >= 0.0 ) {

        vel = osg::clampTo( vel, -m_max_vel, m_max_vel ) ;
    }





    slider->setParam( dParamFMax, m_force ) ;
    slider->setParam( dParamVel, vel ) ;


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SliderServoMotor*
SliderServoMotor::asSliderServoMotor(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
