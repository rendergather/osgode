/*!
 * @file LMotorJoint.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
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

#ifndef _OSGODE_LMOTORJOINT_INL
#define _OSGODE_LMOTORJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::LMotorJoint::setNumAxes(unsigned int num_axes)
{
    dJointSetLMotorNumAxes(m_ODE_joint, num_axes) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODE::LMotorJoint::getNumAxes(void) const
{
    return dJointGetLMotorNumAxes(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::LMotorJoint::setAxis1Anchor(AxisAnchor anchor)
{
    m_x_axis_anchor = anchor ;

    setAxis1( getAxis1() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::LMotorJoint::AxisAnchor
osgODE::LMotorJoint::getAxis1Anchor(void) const
{
    return m_x_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::LMotorJoint::setAxis2Anchor(AxisAnchor anchor)
{
    m_y_axis_anchor = anchor ;

    setAxis2( getAxis2() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::LMotorJoint::AxisAnchor
osgODE::LMotorJoint::getAxis2Anchor(void) const
{
    return m_y_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::LMotorJoint::setAxis3Anchor(AxisAnchor anchor)
{
    m_z_axis_anchor = anchor ;

    setAxis3( getAxis3() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::LMotorJoint::AxisAnchor
osgODE::LMotorJoint::getAxis3Anchor(void) const
{
    return m_z_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_LMOTORJOINT_INL */
