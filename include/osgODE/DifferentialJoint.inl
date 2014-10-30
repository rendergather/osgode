/*!
 * @file DifferentialJoint.inl
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

#ifndef _OSGODE_DIFFERENTIALJOINT_INL
#define _OSGODE_DIFFERENTIALJOINT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DifferentialJoint::setFriction(ooReal friction)
{
    this->setParam(dParamCFM2, -1.0 + 1.0 / friction) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::DifferentialJoint::getFriction(void) const
{
    const ooReal    c = const_cast<DifferentialJoint*>(this)->getParam(dParamCFM2) ;

    return 1.0 / (c + 1.0) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DifferentialJoint::setRatio1(ooReal ratio)
{
    dJointSetDifferentialRatio1(m_ODE_joint, ratio) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::DifferentialJoint::getRatio1(void) const
{
    return dJointGetDifferentialRatio1(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DifferentialJoint::setRatio2(ooReal ratio)
{
    dJointSetDifferentialRatio2(m_ODE_joint, ratio) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::DifferentialJoint::getRatio2(void) const
{
    return dJointGetDifferentialRatio2(m_ODE_joint) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_DIFFERENTIALJOINT_INL */
