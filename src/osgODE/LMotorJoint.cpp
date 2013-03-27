/*!
 * @file LMotorJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
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
#include <osgODE/LMotorJoint>
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
LMotorJoint::LMotorJoint(AxisAnchor default_axis_anchor)
{
    m_ODE_joint = dJointCreateLMotor(StaticWorld::instance()->getODEWorld(), NULL) ;

    setNumAxes(3) ;

    setAxis1Anchor( default_axis_anchor ) ;
    setAxis2Anchor( default_axis_anchor ) ;
    setAxis3Anchor( default_axis_anchor ) ;


    m_functions.SetParam = dJointSetLMotorParam ;
    m_functions.GetParam = dJointGetLMotorParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LMotorJoint::LMotorJoint(const LMotorJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{

    setNumAxes(other.getNumAxes()) ;

    setAxis1Anchor(other.getAxis1Anchor()) ;
    setAxis2Anchor(other.getAxis2Anchor()) ;
    setAxis3Anchor(other.getAxis3Anchor()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LMotorJoint::~LMotorJoint(void)
{
    if(m_ODE_joint) {
        dJointDestroy(m_ODE_joint) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::setAxis1Implementation(const osg::Vec3& axis)
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
        dJointSetLMotorAxis(m_ODE_joint, 0, m_z_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::setAxis2Implementation(const osg::Vec3& axis)
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
        dJointSetLMotorAxis(m_ODE_joint, 1, m_y_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::setAxis3Implementation(const osg::Vec3& axis)
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
        dJointSetLMotorAxis(m_ODE_joint, 2, m_z_axis_anchor, axis.x(), axis.y(), axis.z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::readAxis1Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    dJointGetLMotorAxis(m_ODE_joint, 0, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::readAxis2Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    dJointGetLMotorAxis(m_ODE_joint, 1, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LMotorJoint::readAxis3Implementation(osg::Vec3& axis)
{
    if( ! getWorld() ) {
        return ;
    }

    dVector3    vec ;
    dOPE(vec, =, axis) ;

    dJointGetLMotorAxis(m_ODE_joint, 2, vec) ;

    dOPE(axis, =, vec) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
LMotorJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::LMotorJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreateLMotor(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;



    {
        dJointSetLMotorNumAxes( j, dJointGetLMotorNumAxes(m_ODE_joint) ) ;
    }



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
