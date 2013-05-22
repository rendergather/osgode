/*!
 * @file AMotorJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2013 by Rocco Martino                            *
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
#include <osgODE/AMotorJoint>
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
AMotorJoint::AMotorJoint(void):
    MotorJoint(WORLD, dAMotorUser)
{
    m_ODE_joint = dJointCreateAMotor(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    setNumAxes(3) ;


    m_functions.SetParam = dJointSetAMotorParam ;
    m_functions.GetParam = dJointGetAMotorParam ;

    m_set_axis_fn = dJointSetAMotorAxis ;
    m_get_axis_fn = dJointGetAMotorAxis ;
    m_set_motor_mode_fn = dJointSetAMotorMode ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AMotorJoint::AMotorJoint(const AMotorJoint& other, const osg::CopyOp& copyop):
    MotorJoint(other, copyop)
{
    setNumAxes( other.getNumAxes() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AMotorJoint::~AMotorJoint(void)
{
    if(m_ODE_joint) {
        dJointDestroy(m_ODE_joint) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
AMotorJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::AMotorJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreateAMotor(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;



    {
        dJointSetAMotorNumAxes( j, dJointGetAMotorNumAxes(m_ODE_joint) ) ;
    }



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
