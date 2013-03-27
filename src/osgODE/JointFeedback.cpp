/*!
 * @file JointFeedback.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/JointFeedback>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
JointFeedback::JointFeedback(void)
{
    dOPEC(m_feedback.f1, =, REAL(0.0)) ;
    dOPEC(m_feedback.f2, =, REAL(0.0)) ;
    dOPEC(m_feedback.t1, =, REAL(0.0)) ;
    dOPEC(m_feedback.t2, =, REAL(0.0)) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
JointFeedback::JointFeedback(const JointFeedback& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop)
{
    dOPE(m_feedback.f1, =, other.m_feedback.f1) ;
    dOPE(m_feedback.f2, =, other.m_feedback.f2) ;
    dOPE(m_feedback.t1, =, other.m_feedback.t1) ;
    dOPE(m_feedback.t2, =, other.m_feedback.t2) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
JointFeedback::~JointFeedback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */
