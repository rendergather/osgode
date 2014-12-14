/*!
 * @file PistonJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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
#include <osgODE/PistonJoint>
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
PistonJoint::PistonJoint(void)
{
    m_ODE_joint = dJointCreatePiston(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    m_functions.SetAxis1    = dJointSetPistonAxis ;
    m_functions.GetAxis1    = dJointGetPistonAxis ;

    m_functions.SetAnchor1  = dJointSetPistonAnchor ;
    m_functions.GetAnchor1  = dJointGetPistonAnchor ;

    m_functions.GetAnchor2  = dJointGetPistonAnchor2 ;

    m_functions.SetParam    = dJointSetPistonParam ;
    m_functions.GetParam    = dJointGetPistonParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
PistonJoint::PistonJoint(const PistonJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
PistonJoint::~PistonJoint(void)
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
PistonJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("oo::PistonJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreatePiston(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;




    {
        dVector3    v ;
        dJointGetPistonAxis(m_ODE_joint, v) ;
        dJointSetPistonAxis(j, v[0], v[1], v[2]) ;
    }


    {
        dVector3    a ;
        dJointGetPistonAnchor(m_ODE_joint, a) ;
        dJointSetPistonAnchor(j, a[0], a[1], a[2]) ;
    }



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
