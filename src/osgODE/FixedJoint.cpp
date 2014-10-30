/*!
 * @file FixedJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2013 by Rocco Martino                            *
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
#include <osgODE/FixedJoint>
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
FixedJoint::FixedJoint(void):
    m_oneway_mode       ( false ),
    m_rotation_mode     ( true ),
    m_position_mode     ( true ),
    m_compute_matrix    ( true )
{
    this->setInfo(6, 6, 6) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
FixedJoint::FixedJoint(const FixedJoint& other, const osg::CopyOp& copyop):
    BypassJoint         ( other, copyop ),
    m_oneway_mode       ( other.m_oneway_mode ),
    m_rotation_mode     ( other.m_rotation_mode ),
    m_position_mode     ( other.m_position_mode ),
    m_compute_matrix    ( other.m_compute_matrix ),
    m_matrix            ( other.m_matrix ),
    m_quat              ( other.m_quat ),
    m_pos               ( other.m_pos )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
FixedJoint::~FixedJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
FixedJoint::update( ooReal step_size )
{
    int row = 0 ;


    BypassJoint::BodyMask   mode = m_oneway_mode ? CONSTRAIN_BODY2 : CONSTRAIN_ALL ;


    if( m_rotation_mode ) {

        this->BypassJoint::setRelativeRotation( step_size,
                                                m_quat,
                                                row,
                                                m_erp[0],
                                                m_cfm[0],
                                                mode
                        ) ;
    }




    if( m_position_mode ) {

        this->BypassJoint::setRelativePosition( step_size,
                                                m_pos,
                                                row,
                                                m_erp[0],
                                                m_cfm[0],
                                                mode
                        ) ;
    }



    this->BypassJoint::setInfo( row, row, row ) ;




    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
FixedJoint::finalize(void)
{
    PS_DBG2("osgODE::FixedJoint::finalize(%p)", this) ;


    RigidBody*  b1 = NULL ;
    RigidBody*  b2 = NULL ;


    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1 : NULL ;

    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2 : NULL ;
    }




    if( m_compute_matrix ) {

        if( b1 && b2 ) {
            setMatrix( b2->getMatrix() * osg::Matrix::inverse(b1->getMatrix()) ) ;
        }

        else if( b1 ) {
            setMatrix( b1->getMatrix() ) ;
        }

        else if( b2 ) {
            setMatrix( b2->getMatrix() ) ;
        }
    }


    this->BypassJoint::finalize() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
FixedJoint*
FixedJoint::asFixedJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
