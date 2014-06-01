/*!
 * @file SliderJoint.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2014 by Rocco Martino                            *
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

#ifndef _OSGODE_SLIDERJOINT_INL
#define _OSGODE_SLIDERJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::SliderJoint::getPosition(void) const
{
    return dJointGetSliderPosition(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::SliderJoint::getPositionRate(void) const
{
    return dJointGetSliderPositionRate(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SliderJoint::setAutoComputeRelativeValues( bool auto_compute_relative_values )
{
    dJointSetSliderAutoComputeRelativeValues( m_ODE_joint, auto_compute_relative_values ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::SliderJoint::getAutoComputeRelativeValues(void) const
{
    return dJointGetSliderAutoComputeRelativeValues(m_ODE_joint) != 0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SliderJoint::setRelativeRotation( const osg::Quat& relative_rotation )
{
    dQuaternion q ;

    q[0] = relative_rotation.w() ;
    q[1] = relative_rotation.x() ;
    q[2] = relative_rotation.y() ;
    q[3] = relative_rotation.z() ;

    dJointSetSliderRelativeRotation( m_ODE_joint, q ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Quat
osgODE::SliderJoint::getRelativeRotation(void) const
{
    dQuaternion q ;

    dJointGetSliderRelativeRotation( m_ODE_joint, q ) ;

    return osg::Quat( q[1], q[2], q[3], q[0] ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SliderJoint::setRelativePosition( const osg::Vec3& relative_position )
{
    dVector3    v ;

    v[0] = relative_position.x() ;
    v[1] = relative_position.y() ;
    v[2] = relative_position.z() ;

    dJointSetSliderRelativePosition( m_ODE_joint, v ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::SliderJoint::getRelativePosition(void) const
{
    dVector3    v ;

    dJointGetSliderRelativePosition( m_ODE_joint, v ) ;

    return osg::Vec3( v[0], v[1], v[2] ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_SLIDERJOINT_INL */
