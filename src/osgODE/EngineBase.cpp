/*!
 * @file EngineBase.cpp
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/EngineBase>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
EngineBase::EngineBase(void):
    m_speed         ( 0.0 ),
    m_fmax          ( 0.0 ),
    m_vel           ( 0.0 ),
    m_inertia       ( 1.0 ),
    m_drag          ( 0.0 ),
    m_friction      ( 0.0 ),
    m_ratio         ( 1.0 ),
    m_speed_limit   ( FLT_MAX ),
    m_gas           ( 0.0 ),
    m_speed_stall   ( 0.0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
EngineBase::EngineBase(const EngineBase& other, const osg::CopyOp& copyop):
    osg::Object     ( other, copyop ),
    m_speed         ( other.m_speed ),
    m_fmax          ( other.m_fmax ),
    m_vel           ( other.m_vel ),
    m_inertia       ( other.m_inertia ),
    m_drag          ( other.m_drag ),
    m_friction      ( other.m_friction ),
    m_ratio         ( other.m_ratio ),
    m_speed_limit   ( other.m_speed_limit ),
    m_gas           ( other.m_gas ),
    m_speed_stall   ( other.m_speed_stall )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
EngineBase::~EngineBase(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
EngineBase::propagate( double step_size, double& vel_out, double& fmax_out )
{
    PS_ASSERT1( m_fmax >= 0.0 ) ;
    PS_ASSERT1( m_inertia > 0.0 ) ;
    PS_ASSERT1( m_drag >= 0.0 ) ;


    // idle
    const double    idle_throttle = 1.0 - osg::minimum(m_speed_stall, m_speed) / m_speed_stall ;
    const double    gas = osg::maximum( m_gas, pow( idle_throttle, 1.0/2.0 ) ) ;


    // max speed variation for the powered part
    const double    DV  = m_vel - m_speed ;

    // cache
    const double    I   = step_size / m_inertia ;


    // Disable torque if we're above the speed limit
    const double    apply_torque = (double) ( fabs(m_speed) < m_speed_limit) ;


    // compute gas
    const double    fmax = m_fmax * gas ;


    // sign of the constrained acceleration (caused by vel and fmax)
    const double    constr_accel_sgn    = osg::signOrZero( DV ) ;

    // sign of the free acceleration (caused by drag)
    const double    free_accel_sgn      = osg::signOrZero( m_speed ) * -1.0 ;


    // speed variation caused by the constrained acceleration (cannot exceed DV)
    const double    constr_ds           = osg::minimum( I * fmax, fabs(DV) )  *  apply_torque ;

    // speed variation caused by the free acceleration (cannot exceed current speed)
    const double    free_ds             = osg::minimum( I * m_drag,   fabs(m_speed) ) ;



    // sum both contributes to update the current speed
    m_speed +=      constr_ds * constr_accel_sgn +
                    free_ds   * free_accel_sgn ;




    //
    // restitute angular speed and torque
    //

    const double    EPSILON = 1.0e-3 ;


    if( fabs(m_ratio) > EPSILON ) {
        fmax_out = fabs(fmax - m_drag) * m_friction / fabs(m_ratio) ;
        vel_out = m_speed * m_ratio * ( fmax >= m_drag ) ;
    } else {
        vel_out = 0.0 ;
        fmax_out = 0.0 ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
EngineBase::feedback( double step_size, double speed_in )
{
    PS_ASSERT1( m_friction >= 0.0 ) ;


    const double    EPSILON = 1.0e-3 ;

    if( fabs(m_ratio) > EPSILON ) {


        //  :D

        m_speed =   m_friction          * speed_in / m_ratio  +
                    (1.0 - m_friction)  * m_speed ;

    }
}
/* ....................................................................... */
/* ======================================================================= */
