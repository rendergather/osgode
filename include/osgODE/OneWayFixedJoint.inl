/*!
 * @file OneWayFixedJoint.inl
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

#ifndef _OSGODE_ONEWAYFIXEDJOINT_INL
#define _OSGODE_ONEWAYFIXEDJOINT_INL




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::OneWayFixedJoint::setMatrix( const osg::Matrix& matrix )
{
    m_matrix = matrix ;


    osg::Quat   so ;
    osg::Vec3   s ;

    matrix.decompose(m_pos, m_quat, s, so) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Matrix&
osgODE::OneWayFixedJoint::getMatrix(void) const
{
    return m_matrix ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::OneWayFixedJoint::setConstrainRotation( bool rotation_mode )
{
    m_rotation_mode = rotation_mode ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::OneWayFixedJoint::getConstrainRotation(void) const
{
    return m_rotation_mode ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::OneWayFixedJoint::setQuaternion( const osg::Quat& quaternion )
{
    m_quat = quaternion ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Quat&
osgODE::OneWayFixedJoint::getQuaternion(void) const
{
    return m_quat ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::OneWayFixedJoint::setPosition( const osg::Vec3& position )
{
    m_pos = position ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::OneWayFixedJoint::getPosition(void) const
{
    return m_pos ;
}
/* ....................................................................... */
/* ======================================================================= */





#endif /* _OSGODE_ONEWAYFIXEDJOINT_INL */
