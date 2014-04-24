/*!
 * @file HingeJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2014 by Rocco Martino                            *
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
#include <osgODE/HingeJoint>
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
HingeJoint::HingeJoint(void)
{
    m_ODE_joint = dJointCreateHinge(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    m_functions.SetAxis1    = dJointSetHingeAxis ;
    m_functions.GetAxis1    = dJointGetHingeAxis ;

    m_functions.SetAnchor1  = dJointSetHingeAnchor ;
    m_functions.GetAnchor1  = dJointGetHingeAnchor ;

    m_functions.GetAnchor2  = dJointGetHingeAnchor2 ;

    m_functions.SetParam    = dJointSetHingeParam ;
    m_functions.GetParam    = dJointGetHingeParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HingeJoint::HingeJoint(const HingeJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HingeJoint::~HingeJoint(void)
{
    if(m_ODE_joint) {
        dJointDestroy(m_ODE_joint) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HingeJoint*
HingeJoint::asHingeJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
HingeJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::HingeJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreateHinge(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;



    {
        dVector3    v ;
        dJointGetHingeAnchor(m_ODE_joint, v) ;
        dJointSetHingeAnchor(j, v[0], v[1], v[2]) ;
    }

    {
        dVector3    v ;
        dJointGetHingeAxis(m_ODE_joint, v) ;
        dJointSetHingeAxis(j, v[0], v[1], v[2]) ;
    }


    {
        dQuaternion     qrel ;

        dJointSetHingeAutoComputeRelativeValues( j,      dJointGetHingeAutoComputeRelativeValues(m_ODE_joint) ) ;

        dJointGetHingeRelativeRotation( m_ODE_joint, qrel ) ;

        dJointSetHingeRelativeRotation( j, qrel ) ;
    }



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
