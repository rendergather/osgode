/*!
 * @file MotorJoint.inl
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

#ifndef _OSGODE_MOTORJOINT_INL
#define _OSGODE_MOTORJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::MotorJoint::setMotorMode(int mode)
{
    m_motor_mode = mode ;

    if( getWorld()  &&  m_set_motor_mode_fn ) {
        m_set_motor_mode_fn(m_ODE_joint, mode) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline int
osgODE::MotorJoint::getMotorMode(void) const
{
    return m_motor_mode ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::MotorJoint::setAxis1Anchor(AxisAnchor anchor)
{
    m_x_axis_anchor = anchor ;

    setAxis1( getAxis1() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::MotorJoint::AxisAnchor
osgODE::MotorJoint::getAxis1Anchor(void) const
{
    return m_x_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::MotorJoint::setAxis2Anchor(AxisAnchor anchor)
{
    m_y_axis_anchor = anchor ;

    setAxis2( getAxis2() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::MotorJoint::AxisAnchor
osgODE::MotorJoint::getAxis2Anchor(void) const
{
    return m_y_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::MotorJoint::setAxis3Anchor(AxisAnchor anchor)
{
    m_z_axis_anchor = anchor ;

    setAxis3( getAxis3() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::MotorJoint::AxisAnchor
osgODE::MotorJoint::getAxis3Anchor(void) const
{
    return m_z_axis_anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_MOTORJOINT_INL */
