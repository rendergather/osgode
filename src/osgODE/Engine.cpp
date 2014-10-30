/*!
 * @file Engine.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Engine>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Engine::Engine(void):
    m_current_gear  (0)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Engine::Engine(const Engine& other, const osg::CopyOp& copyop):
    EngineBase(other, copyop),

    m_torque_curve  ( osg::clone(other.m_torque_curve.get(), copyop) ),
    m_gear_list     ( other.m_gear_list ),
    m_current_gear  ( other.m_current_gear )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Engine::~Engine(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Engine::propagate( ooReal step_size, ooReal& vel_out, ooReal& fmax_out )
{

    // find current torque
    //
    if( m_torque_curve.valid() ) {

        const ooReal    fmax = m_torque_curve->interpolate( getSpeed() ) ;

        setFMax( fmax ) ;
        setVel( FLT_MAX ) ;
    }





    if( m_gear_list.size() > 0 ) {

        m_current_gear = osg::minimum( m_current_gear, (unsigned int)(m_gear_list.size() - 1) ) ;

        const ooReal    current_ratio = getRatio() ;
        const ooReal    new_ratio = m_gear_list[m_current_gear] ;



        const ooReal    N = 10 ;
        const ooReal    interpolated_ratio = (current_ratio * N + new_ratio) / (N+1) ;


        const ooReal    EPSILON = 1.0e-3 ;


        if( fabs(new_ratio) > EPSILON   &&   fabs(current_ratio) > EPSILON ) {

            setRatio( interpolated_ratio ) ;

        } else {

            setRatio( new_ratio ) ;
        }
    }



    this->EngineBase::propagate( step_size, vel_out, fmax_out ) ;
}
/* ....................................................................... */
/* ======================================================================= */
