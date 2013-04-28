/*!
 * @file DifferentialJoint.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/DifferentialJoint>
#include <osgODE/StaticWorld>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
DifferentialJoint::DifferentialJoint(void)
{
    m_ODE_joint = dJointCreateDifferential(StaticWorld::instance()->getODEWorld(), NULL) ;

    m_functions.SetAxis1    = dJointSetDifferentialAxis1 ;
    m_functions.GetAxis1    = dJointGetDifferentialAxis1 ;

    m_functions.SetAxis2    = dJointSetDifferentialAxis2 ;
    m_functions.GetAxis2    = dJointGetDifferentialAxis2 ;

    m_functions.SetParam    = dJointSetDifferentialParam ;
    m_functions.GetParam    = dJointGetDifferentialParam ;


    this->Joint::m_ensure_two_bodies = true ;


    this->setParam( dParamCFM2, dInfinity ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DifferentialJoint::DifferentialJoint(const DifferentialJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DifferentialJoint::~DifferentialJoint(void)
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
DifferentialJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::DifferentialJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateDifferential(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    dJointSetDifferentialRatio1( j, dJointGetDifferentialRatio1(m_ODE_joint) ) ;
    dJointSetDifferentialRatio2( j, dJointGetDifferentialRatio2(m_ODE_joint) ) ;


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
