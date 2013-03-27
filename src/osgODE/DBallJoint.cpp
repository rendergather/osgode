/*!
 * @file DBallJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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
#include <osgODE/DBallJoint>
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
DBallJoint::DBallJoint(void)
{
    m_ODE_joint = dJointCreateDBall(StaticWorld::instance()->getODEWorld(), NULL) ;

    m_functions.SetAnchor1  = dJointSetDBallAnchor1 ;
    m_functions.GetAnchor1  = dJointGetDBallAnchor1 ;

    m_functions.SetAnchor2  = dJointSetDBallAnchor2 ;
    m_functions.GetAnchor2  = dJointGetDBallAnchor2 ;

    m_functions.SetParam    = dJointSetDBallParam ;
    m_functions.GetParam    = dJointGetDBallParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DBallJoint::DBallJoint(const DBallJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DBallJoint::~DBallJoint(void)
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
DBallJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::DBallJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateDBall(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    dVector3    anchor ;
    dJointGetDBallAnchor1(m_ODE_joint, anchor) ;
    dJointSetDBallAnchor1(j, anchor[0], anchor[1], anchor[2]) ;
    dJointGetDBallAnchor2(m_ODE_joint, anchor) ;
    dJointSetDBallAnchor2(j, anchor[0], anchor[1], anchor[2]) ;


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
