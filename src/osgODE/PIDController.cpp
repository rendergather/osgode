/*!
 * @file PIDController.cpp
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
#include <osgODE/PIDController>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
PIDController::PIDController(void):
    m_proportional          ( 1.0 ),
    m_integral              ( 0.0 ),
    m_derivative            ( 0.0 ),
    m_error_internal        ( 0.0 ),
    m_accum                 ( 0.0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
PIDController::PIDController(const PIDController& other, const osg::CopyOp& copyop):
    osg::Object             ( other, copyop ),
    m_proportional          ( other.m_proportional ),
    m_integral              ( other.m_integral ),
    m_derivative            ( other.m_derivative ),
    m_error_internal        ( other.m_error_internal ),
    m_accum                 ( other.m_accum )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
PIDController::~PIDController(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
double
PIDController::solve( double error, double dt )
{
    m_accum += 0.5 * ( error + m_error_internal ) * dt ;

    double  deriv = ( error - m_error_internal ) / dt ;

    m_error_internal = error ;

    return m_integral * m_accum + m_derivative * deriv + m_proportional * error ;
}
/* ....................................................................... */
/* ======================================================================= */
