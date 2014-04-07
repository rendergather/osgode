/*!
 * @file Car.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "Car"

#include <osgODE/Engine>
#include <osgODE/Cylinder>
#include <osgODE/SuspensionJoint>
#include <osgODE/DifferentialJoint>




using namespace osgODE ;




Car::Car(void):
    m_steering_left(false),
    m_steering_right(false),
    m_current_steer_angle(0.0)
{
}


Car::Car(const Car& other, const osg::CopyOp& copyop):
    Container(other, copyop),
    m_steering_left(other.m_steering_left),
    m_steering_right(other.m_steering_right),
    m_current_steer_angle(other.m_current_steer_angle)
{
}




Car::~Car(void)
{
}




void
Car::update(double step_size)
{
    _applyTorque( step_size ) ;
    _applySteer( step_size ) ;
}









void
Car::_applyTorque(double step_size)
{
    //
    // apply the torque to the differentials
    //




    double  vel, fmax ;



    // really need a third differential
    //
    const double    vel_in = ( m_front_differential->getAngleRate() + m_rear_differential->getAngleRate() ) * 0.5 ;


    m_engine->feedback( step_size, vel_in ) ;
    m_engine->propagate( step_size, vel, fmax ) ;





    // torque balance       0 -> 1  :  rear -> front
    const double    BALANCE = 3.0/7.0 ;

    m_front_differential->setParam( dParamVel, vel ) ;
    m_front_differential->setParam( dParamFMax, fmax * BALANCE ) ;

    m_rear_differential->setParam( dParamVel, vel ) ;
    m_rear_differential->setParam( dParamFMax, fmax * (1.0 - BALANCE) ) ;
}









void
Car::_applySteer(double step_size)
{
    // modulus of the maximum rotation angle
    const double    MAX_STEER = (1.0/6.0) * osg::PI ;

    // Sensitivity
    const double    SENSITIVITY = 2.0 ;

    // The wheels will steer gently
    // This variable holds the maximum allowed rotation for the current step
    const double    ds = MAX_STEER * step_size * SENSITIVITY ;


    const double    UNDERSTEER_MAX = 0.2 ;

    // Is the vehicle understeering too much?
    const bool      understeer = _computeUnderSteer() >= UNDERSTEER_MAX ;






    // At standard condition, the wheels keep rotate until they reach
    // the maximum angle.
    // If the car is understeering, we ease the steering wheel to give
    // directionality to the forecarriage



    switch( m_steering_left  |  (m_steering_right << 1) )
    {
        case 0:
        // The user is not steering at all
        // This simply brings back the angle to 0
        {
            if( m_current_steer_angle > 0.0 ) {
                m_current_steer_angle -= ds * 2.0 ;

                m_current_steer_angle = osg::maximum(0.0, m_current_steer_angle) ;
            } else {
                m_current_steer_angle += ds * 2.0 ;

                m_current_steer_angle = osg::minimum(0.0, m_current_steer_angle) ;
            }
        }
        break ;




        case 1:
        // The user is steering to the left
        {
            if( understeer ) {
                m_current_steer_angle += ds ;

                // This minimum angle is always allowed
                // Note that commenting out the following line results in an oversteer correction, too
                m_current_steer_angle = osg::minimum(-ds * 4.0, m_current_steer_angle) ;

            } else {
                m_current_steer_angle -= ds ;
            }
        }
        break ;




        case 2:
        // The user is steering to the right
        {
            if( understeer ) {
                m_current_steer_angle -= ds ;

                // This minimum angle is always allowed
                // Note that commenting out the following line results in an oversteer correction, too
                m_current_steer_angle = osg::maximum(ds * 4.0, m_current_steer_angle) ;

            } else {
                m_current_steer_angle += ds ;
            }
        }
        break ;




        case 3:
        // stering left and right: do nothing
        break ;
    }





    // Ensure the angle is in the range
    m_current_steer_angle = osg::clampTo(m_current_steer_angle, -MAX_STEER, +MAX_STEER) ;



    // done :)
    m_hinge_FL->setSteerAngle(m_current_steer_angle) ;
    m_hinge_FR->setSteerAngle(m_current_steer_angle) ;



    this->Container::update(step_size) ;
}








// I estimate the understeer by making the scalar product between the
// vehicle direction and the wheel rotation axis. The sign of the product
// is finally modified to indicate the understeer when it's positive


double
Car::_computeUnderSteer(void)
{
    const double    steer_sign = (int)m_steering_left - (int)m_steering_right ;


    // This point is in local coord. I choose it because it's between the two wheels
    osg::Vec3   dir = m_body->getPointVelocity( osg::Vec3(0.0, 1.5, 0.0), true ) ;

    // subtract because the left axis is opposite to the right one
    osg::Vec3   axis = m_hinge_FR->getAxis2() - m_hinge_FL->getAxis2() ;

    dir.normalize() ;
    axis.normalize() ;

    double  dot = axis * dir ;


    return dot * steer_sign ;
}
