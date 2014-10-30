/*!
 * @file BypassJoint.inl
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
osgODE::BypassJoint::setERP( int i, ooReal v)
{
    m_erp[i] = v ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::BypassJoint::getERP( int i )
{
    return m_erp[i] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::setCFM( int i, ooReal v)
{
    m_cfm[i] = v ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::BypassJoint::getCFM( int i )
{
    return m_cfm[i] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::setInfo(   unsigned int max_m,
                                unsigned int m,
                                unsigned int nub )
{
    dJointSetBypassInfo(m_ODE_joint, max_m, m, nub) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::BypassJoint::getInfo(   unsigned int& max_m,
                                unsigned int& m,
                                unsigned int& nub ) const
{
    dJointGetBypassInfo(m_ODE_joint, &max_m, &m, &nub) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_BYPASSJOINT_INL */
