/*!
 * @file JointFeedback.inl
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

#ifndef _OSGODE_JOINTFEEDBACK_INL
#define _OSGODE_JOINTFEEDBACK_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::JointFeedback::getF1(void) const
{
    return osg::Vec3(m_feedback.f1[0], m_feedback.f1[1], m_feedback.f1[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::JointFeedback::getF2(void) const
{
    return osg::Vec3(m_feedback.f2[0], m_feedback.f2[1], m_feedback.f2[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::JointFeedback::getT1(void) const
{
    return osg::Vec3(m_feedback.t1[0], m_feedback.t1[1], m_feedback.t1[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::JointFeedback::getT2(void) const
{
    return osg::Vec3(m_feedback.t2[0], m_feedback.t2[1], m_feedback.t2[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline dJointFeedback*
osgODE::JointFeedback::getODEJointFeedbackInternal(void)
{
    return &m_feedback ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_JOINTFEEDBACK_INL */
