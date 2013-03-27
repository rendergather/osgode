/*!
 * @file TargetJoint.inl
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

#ifndef _OSGODE_TARGETJOINT_INL
#define _OSGODE_TARGETJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::TargetJoint::setPosition(const osg::Vec3& position)
{
    dJointSetTargetPosition(    m_ODE_joint,
                                dJointGetTargetPositionTime( m_ODE_joint ),
                                position.x(),
                                position.y(),
                                position.z()
                           ) ;



    m_position = position ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::TargetJoint::getPosition(void) const
{
    return m_position ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::TargetJoint::setPositionTime(double time)
{
    dVector3    result ;
    dJointGetTargetPosition( m_ODE_joint, result ) ;

    dJointSetTargetPosition(    m_ODE_joint,
                                time,
                                result[0],
                                result[1],
                                result[2]
                           ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::TargetJoint::getPositionTime(void) const
{
    return dJointGetTargetPositionTime( m_ODE_joint ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::TargetJoint::setQuaternion(const osg::Quat& quaternion)
{
    dQuaternion q ;

    q[0] = quaternion.w() ;
    q[1] = quaternion.x() ;
    q[2] = quaternion.y() ;
    q[3] = quaternion.z() ;

    dJointSetTargetQuaternion( m_ODE_joint, dJointGetTargetQuaternionTime( m_ODE_joint ), q ) ;


    m_quaternion = quaternion ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Quat&
osgODE::TargetJoint::getQuaternion(void) const
{
    return m_quaternion ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::TargetJoint::setQuaternionTime(double time)
{
    dQuaternion result ;
    dJointGetTargetQuaternion( m_ODE_joint, result ) ;

    dJointSetTargetQuaternion( m_ODE_joint, time, result ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::TargetJoint::getQuaternionTime(void) const
{
    return dJointGetTargetQuaternionTime( m_ODE_joint ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_TARGETJOINT_INL */
