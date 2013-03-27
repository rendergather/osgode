/*!
 * @file JointBreakCallback.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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

#ifndef _OSGODE_JOINTBREAKCALLBACK_INL
#define _OSGODE_JOINTBREAKCALLBACK_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::JointBreakCallback::setEnabled(bool flag)
{
    m_enabled = flag ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::JointBreakCallback::getEnabled(void) const
{
    return m_enabled ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::JointBreakCallback::setForceLimit(double limit)
{
    m_force_limit = limit ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::JointBreakCallback::getForceLimit(void) const
{
    return m_force_limit ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::JointBreakCallback::setTorqueLimit(double limit)
{
    m_torque_limit = limit ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::JointBreakCallback::getTorqueLimit(void) const
{
    return m_torque_limit ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::JointBreakCallback::setRemoveOnBreak(bool flag)
{
    m_remove_on_break = flag ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::JointBreakCallback::getRemoveOnBreak(void) const
{
    return m_remove_on_break ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_JOINTBREAKCALLBACK_INL */
