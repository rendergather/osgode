/*!
 * @file DHingeJoint.cpp
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
#include <osgODE/DHingeJoint>
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
DHingeJoint::DHingeJoint(void)
{
    m_ODE_joint = dJointCreateDHinge(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    m_functions.SetAnchor1  = dJointSetDHingeAnchor1 ;
    m_functions.GetAnchor1  = dJointGetDHingeAnchor1 ;

    m_functions.SetAnchor2  = dJointSetDHingeAnchor2 ;
    m_functions.GetAnchor2  = dJointGetDHingeAnchor2 ;

    m_functions.SetAxis1    = dJointSetDHingeAxis ;
    m_functions.GetAxis1    = dJointGetDHingeAxis ;

    m_functions.SetParam    = dJointSetDHingeParam ;
    m_functions.GetParam    = dJointGetDHingeParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DHingeJoint::DHingeJoint(const DHingeJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DHingeJoint::~DHingeJoint(void)
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
DHingeJoint::finalize(void)
{
    PS_DBG2("oo::DHingeJoint::finalize(%p)", this) ;



    dBodyID b1 ;
    dBodyID b2 ;



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



    if( b1 || b2 ) {

        dJointAttach(m_ODE_joint, b1, b2) ;

        _restoreParams() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
DHingeJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("oo::DHingeJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateDHinge(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    {
        dVector3    anchor ;

        dJointGetDHingeAnchor1(m_ODE_joint, anchor) ;
        dJointSetDHingeAnchor1(j, anchor[0], anchor[1], anchor[2]) ;

        dJointGetDHingeAnchor2(m_ODE_joint, anchor) ;
        dJointSetDHingeAnchor2(j, anchor[0], anchor[1], anchor[2]) ;
    }


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
