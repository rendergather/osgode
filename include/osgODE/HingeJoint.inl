/*!
 * @file HingeJoint.inl
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

#ifndef _OSGODE_HINGEJOINT_INL
#define _OSGODE_HINGEJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::HingeJoint::getAngle(void) const
{
    return dJointGetHingeAngle(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::HingeJoint::getAngleRate(void) const
{
    return dJointGetHingeAngleRate(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::HingeJoint::setAutoComputeRelativeValues( bool auto_compute_relative_values )
{
    dJointSetHingeAutoComputeRelativeValues( m_ODE_joint, auto_compute_relative_values ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::HingeJoint::getAutoComputeRelativeValues(void) const
{
    return dJointGetHingeAutoComputeRelativeValues(m_ODE_joint) != 0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::HingeJoint::setRelativeRotation( const osg::Quat& relative_rotation )
{
    dQuaternion q ;

    q[0] = relative_rotation.w() ;
    q[1] = relative_rotation.x() ;
    q[2] = relative_rotation.y() ;
    q[3] = relative_rotation.z() ;

    dJointSetHingeRelativeRotation( m_ODE_joint, q ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Quat
osgODE::HingeJoint::getRelativeRotation(void) const
{
    dQuaternion q ;

    dJointGetHingeRelativeRotation( m_ODE_joint, q ) ;

    return osg::Quat( q[1], q[2], q[3], q[0] ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_HINGEJOINT_INL */
