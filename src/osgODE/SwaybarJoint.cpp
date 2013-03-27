/*!
 * @file SwaybarJoint.cpp
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
#include <osgODE/SwaybarJoint>
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
SwaybarJoint::SwaybarJoint(void):
    m_reference_axis( osg::Z_AXIS ),
    m_reference_axis_local( osg::Z_AXIS )
{
    m_ODE_joint = dJointCreateSwaybar(StaticWorld::instance()->getODEWorld(), NULL) ;

    m_functions.SetAxis1    = dJointSetSwaybarAxis ;
    m_functions.GetAxis1    = dJointGetSwaybarAxis ;

    m_functions.SetParam    = dJointSetSwaybarParam ;
    m_functions.GetParam    = dJointGetSwaybarParam ;



    setParam(dParamLoStop, 0.0) ;
    setParam(dParamHiStop, 0.0) ;
    setParam(dParamERP, 1.0) ;
    setParam(dParamCFM, 0.0) ;
    setParam(dParamStopERP, 1.0) ;
    setParam(dParamStopCFM, 0.0) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SwaybarJoint::SwaybarJoint(const SwaybarJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop),
    m_reference_body( other.m_reference_body ),
    m_reference_axis( other.m_reference_axis ),
    m_reference_axis_local( other.m_reference_axis_local )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SwaybarJoint::~SwaybarJoint(void)
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
SwaybarJoint::update(double step_size)
{

    if( m_reference_body.valid() ) {

        m_reference_axis = m_reference_body->getQuaternion() * m_reference_axis_local ;

    } else {

        m_reference_axis_local = m_reference_axis ;

    }

    this->setAxis1( m_reference_axis ) ;



    this->Joint::update(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
SwaybarJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::SwaybarJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateSwaybar(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    dVector3    axis ;
    dJointGetSwaybarAxis( m_ODE_joint, axis ) ;
    dJointSetSwaybarAxis( j, axis[0], axis[1], axis[2] ) ;


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
