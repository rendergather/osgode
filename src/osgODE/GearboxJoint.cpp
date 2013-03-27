/*!
 * @file GearboxJoint.cpp
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
#include <osgODE/GearboxJoint>
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
GearboxJoint::GearboxJoint(void)
{
    m_ODE_joint = dJointCreateGearbox(StaticWorld::instance()->getODEWorld(), NULL) ;

    m_functions.SetAxis1    = dJointSetGearboxAxis1 ;
    m_functions.GetAxis1    = dJointGetGearboxAxis1 ;

    m_functions.SetAxis2    = dJointSetGearboxAxis2 ;
    m_functions.GetAxis2    = dJointGetGearboxAxis2 ;

    m_functions.SetParam    = dJointSetGearboxParam ;
    m_functions.GetParam    = dJointGetGearboxParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
GearboxJoint::GearboxJoint(const GearboxJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
GearboxJoint::~GearboxJoint(void)
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
GearboxJoint::finalize(void)
{
    PS_DBG2("osgODE::GearboxJoint::finalize(%p)", this) ;


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




    if(b1 && b2) {
        dJointAttach(m_ODE_joint, b1, b2) ;

        _restoreParams() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
GearboxJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::GearboxJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateGearbox(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    dJointSetGearboxRatio( j, dJointGetGearboxRatio(m_ODE_joint) ) ;


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
