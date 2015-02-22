/*!
 * @file BypassJoint.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 - 2015 by Rocco Martino                            *
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
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _OSGODE_BYPASSJOINT_INL
#define _OSGODE_BYPASSJOINT_INL




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::setInfo1(  unsigned int m,
                                unsigned int nub )
{
    dJointSetBypassInfo1(m_ODE_joint, m, nub) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::getInfo1(  unsigned int& m,
                                unsigned int& nub ) const
{
    dJointGetBypassInfo1(m_ODE_joint, &m, &nub) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::setSureMaxInfo(  unsigned int max_m )
{
    dJointSetBypassSureMaxInfo(m_ODE_joint, max_m) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::getSureMaxInfo(  unsigned int& max_m ) const
{
    dJointGetBypassSureMaxInfo(m_ODE_joint, &max_m ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::setInitialTransformation( const osg::Matrix& initial_transformation )
{
    m_initial_transformation = initial_transformation ;
    m_initial_transformation_set = true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Matrix&
osgODE::BypassJoint::getInitialTransformation(void)
{
    return m_initial_transformation ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Matrix&
osgODE::BypassJoint::getInitialTransformation(void) const
{
    return m_initial_transformation ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_BYPASSJOINT_INL */
