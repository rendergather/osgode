/*!
 * @file Hinge2Joint.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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

#ifndef _OSGODE_SUSPENSIONJOINT_INL
#define _OSGODE_SUSPENSIONJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setSpring(ooReal spring)
{
    m_spring = spring ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::SuspensionJoint::getSpring(void) const
{
    return m_spring ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setDamperBound(ooReal damp)
{
    m_damp_bound = damp ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::SuspensionJoint::getDamperBound(void) const
{
    return m_damp_bound ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setDamperRebound(ooReal damp)
{
    m_damp_rebound = damp ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::SuspensionJoint::getDamperRebound(void) const
{
    return m_damp_rebound ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setPreload(ooReal preload)
{
    m_preload = preload ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::SuspensionJoint::getPreload(void) const
{
    return m_preload ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setDamp(ooReal damp)
{
    setDamperBound(damp) ;
    setDamperRebound(damp) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::SuspensionJoint::setSteerAngle(ooReal angle)
{
    setParam(dParamLoStop, angle) ;
    setParam(dParamHiStop, angle) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::SuspensionJoint::getSteerAngle(void) const
{
    SuspensionJoint*    joint = const_cast<SuspensionJoint*>(this) ;
    return 0.5 *   ( joint->getParam(dParamLoStop) + joint->getParam(dParamHiStop) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_SUSPENSIONJOINT_INL */
