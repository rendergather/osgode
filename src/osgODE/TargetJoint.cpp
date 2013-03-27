/*!
 * @file TargetJoint.cpp
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
#include <osgODE/TargetJoint>
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
TargetJoint::TargetJoint(void)
{
    m_ODE_joint = dJointCreateTarget(StaticWorld::instance()->getODEWorld(), NULL) ;

    m_functions.SetParam    = dJointSetTargetParam ;
    m_functions.GetParam    = dJointGetTargetParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TargetJoint::TargetJoint(const TargetJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop),
    m_position( other.m_position ),
    m_quaternion( other.m_quaternion )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TargetJoint::~TargetJoint(void)
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
TargetJoint::postUpdate(double step_size)
{
    osg::Matrix m = osg::Matrix( m_quaternion ) ;

    m(3,0) = m_position.x() ;
    m(3,1) = m_position.y() ;
    m(3,2) = m_position.z() ;


    this->getMatrixTransform()->setMatrix( m ) ;
    this->getMatrixTransform()->dirtyBound() ;



    this->Joint::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TargetJoint*
TargetJoint::asTargetJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
TargetJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::TargetJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateTarget(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    dVector3        position ;
    dQuaternion     quaternion ;

    dJointGetTargetPosition( m_ODE_joint, position ) ;
    dJointGetTargetQuaternion( m_ODE_joint, quaternion ) ;

    dReal           position_time = dJointGetTargetPositionTime( m_ODE_joint ) ;
    dReal           quaternion_time = dJointGetTargetQuaternionTime( m_ODE_joint ) ;

    dJointSetTargetPosition( j, position_time, position[0], position[1], position[2] ) ;
    dJointSetTargetQuaternion( j, quaternion_time, quaternion ) ;


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
