/*!
 * @file MotorJoint.cpp
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
#include <osgODE/MotorJoint>
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
MotorJoint::MotorJoint(AxisAnchor default_axis_anchor, int default_motor_mode)
{
    m_set_axis_fn = NULL ;
    m_get_axis_fn = NULL ;
    m_set_motor_mode_fn = NULL ;

    setMotorMode( default_motor_mode ) ;

    setAxis1Anchor(default_axis_anchor) ;
    setAxis2Anchor(default_axis_anchor) ;
    setAxis3Anchor(default_axis_anchor) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotorJoint::MotorJoint(const MotorJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
    m_set_axis_fn = other.m_set_axis_fn ;
    m_get_axis_fn = other.m_get_axis_fn ;
    m_set_motor_mode_fn = other.m_set_motor_mode_fn ;

    setAxis1Anchor( other.getAxis1Anchor() ) ;
    setAxis2Anchor( other.getAxis2Anchor() ) ;
    setAxis3Anchor( other.getAxis3Anchor() ) ;

    setMotorMode( other.m_motor_mode ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotorJoint::~MotorJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::finalize(void)
{
    PS_DBG2("osgODE::MotorJoint::finalize(%p)", this) ;



    dBodyID b1 = NULL ;
    dBodyID b2 = NULL ;



    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;
    }



    dJointAttach(m_ODE_joint, b1, b2) ;


    if( b1 ) {
        _restoreParams() ;

        setMotorMode(m_motor_mode) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::setAxis1Implementation(const osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }




    dBodyID b1 =  NULL ;
    dBodyID b2 =  NULL ;


    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;

    } else {
        b1 = NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;

    } else {
        b2 = NULL ;
    }



    if( (m_x_axis_anchor == WORLD)  ||  ((m_x_axis_anchor == BODY1) && b1)  ||  ((m_x_axis_anchor == BODY2) && b2) ) {
        m_set_axis_fn(m_ODE_joint, 0, m_x_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::setAxis2Implementation(const osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }




    dBodyID b1 =  NULL ;
    dBodyID b2 =  NULL ;


    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;

    } else {
        b1 = NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;

    } else {
        b2 = NULL ;
    }


    if( (m_y_axis_anchor == WORLD)  ||  ((m_y_axis_anchor == BODY1) && b1)  ||  ((m_y_axis_anchor == BODY2) && b2) ) {
        m_set_axis_fn(m_ODE_joint, 1, m_y_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::setAxis3Implementation(const osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }




    dBodyID b1 =  NULL ;
    dBodyID b2 =  NULL ;


    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;

    } else {
        b1 = NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;

    } else {
        b2 = NULL ;
    }


    if( (m_z_axis_anchor == WORLD)  ||  ((m_z_axis_anchor == BODY1) && b1)  ||  ((m_z_axis_anchor == BODY2) && b2) ) {
        m_set_axis_fn(m_ODE_joint, 2, m_z_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::readAxis1Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    m_get_axis_fn(m_ODE_joint, 0, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::readAxis2Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    m_get_axis_fn(m_ODE_joint, 1, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotorJoint::readAxis3Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    m_get_axis_fn(m_ODE_joint, 2, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */
