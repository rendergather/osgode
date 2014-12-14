/*!
 * @file LMotorJoint.cpp
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
LMotorJoint::LMotorJoint(void):
    MotorJoint(WORLD, 0)
{
    m_ODE_joint = dJointCreateLMotor(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    setNumAxes(3) ;


    m_functions.SetParam = dJointSetLMotorParam ;
    m_functions.GetParam = dJointGetLMotorParam ;

    m_set_axis_fn = dJointSetLMotorAxis ;
    m_get_axis_fn = dJointGetLMotorAxis ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LMotorJoint::LMotorJoint(const LMotorJoint& other, const osg::CopyOp& copyop):
    MotorJoint(other, copyop)
{

    setNumAxes( other.getNumAxes() ) ;
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
dJointID
LMotorJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("oo::LMotorJoint::cloneODEJoint(%p, world=%p)", this, world) ;

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
