/*!
 * @file LMPlusJoint.cpp
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
#include <osgODE/LMPlusJoint>
#include <osgODE/StaticWorld>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    void    dJointSetLMPlusMode3(dJointID j, int mode)
    {
        dJointSetLMPlusMode(j, 0, mode) ;
        dJointSetLMPlusMode(j, 1, mode) ;
        dJointSetLMPlusMode(j, 2, mode) ;
    }
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
LMPlusJoint::LMPlusJoint(void):
    MotorJoint(WORLD, dLMPlusPlanar)
{
    m_ODE_joint = dJointCreateLMPlus(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;


    m_functions.SetParam = dJointSetLMPlusParam ;
    m_functions.GetParam = dJointGetLMPlusParam ;

    m_set_axis_fn = dJointSetLMPlusAxis ;
    m_get_axis_fn = dJointGetLMPlusAxis ;
    m_set_motor_mode_fn = dJointSetLMPlusMode3 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LMPlusJoint::LMPlusJoint(const LMPlusJoint& other, const osg::CopyOp& copyop):
    MotorJoint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LMPlusJoint::~LMPlusJoint(void)
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
LMPlusJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("oo::LMPlusJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreateLMPlus(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
