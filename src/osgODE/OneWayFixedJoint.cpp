/*!
 * @file OneWayFixedJoint.cpp
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
#include <osgODE/OneWayFixedJoint>
#include <osgODE/StaticWorld>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
OneWayFixedJoint::OneWayFixedJoint(void)
{
    this->setInfo(6, 6, 6) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
OneWayFixedJoint::OneWayFixedJoint(const OneWayFixedJoint& other, const osg::CopyOp& copyop):
    BypassJoint ( other, copyop ),
    m_matrix    ( other.m_matrix ),
    m_quat      ( other.m_quat ),
    m_pos       ( other.m_pos )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
OneWayFixedJoint::~OneWayFixedJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
OneWayFixedJoint::update( double step_size )
{
    int row = 0 ;


    this->BypassJoint::setRelativeRotation( step_size,
                                            m_quat,
                                            row,
                                            m_erp[0],
                                            m_cfm[0],
                                            CONSTRAIN_BODY2
                       ) ;



    this->BypassJoint::setRelativePosition( step_size,
                                            m_pos,
                                            row,
                                            m_erp[0],
                                            m_cfm[0],
                                            CONSTRAIN_BODY2
                       ) ;



    this->BypassJoint::setInfo( row, row, row ) ;




    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */
