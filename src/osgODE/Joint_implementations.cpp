/*!
 * @file Joint_implementations.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2012 by Rocco Martino                            *
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
#include <osgODE/Joint>
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
void
Joint::setParamImplementation(int param, double value)
{
    if( m_functions.SetParam ) {
        m_functions.SetParam(m_ODE_joint, param, static_cast<dReal>(value)) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
double
Joint::getParamImplementation(int param)
{
    if( m_functions.GetParam ) {
        return m_functions.GetParam(m_ODE_joint, param) ;
    }

    return 0.0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#define SET_VEC3_IMPLEMENTATION(NAME) \
void \
Joint::set##NAME##Implementation(const osg::Vec3& value) \
{ \
    if( ! getWorld() ) { \
        return ; \
    } \
    if( m_functions.Set##NAME ) { \
        m_functions.Set##NAME(m_ODE_joint, value.x(), value.y(), value.z()) ; \
    } \
}

SET_VEC3_IMPLEMENTATION(Axis1) ;
SET_VEC3_IMPLEMENTATION(Axis2) ;
SET_VEC3_IMPLEMENTATION(Axis3) ;
SET_VEC3_IMPLEMENTATION(Anchor1) ;
SET_VEC3_IMPLEMENTATION(Anchor2) ;

#undef SET_VEC3_IMPLEMENTATION
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#define READ_VEC3_IMPLEMENTATION(NAME) \
void \
Joint::read##NAME##Implementation(osg::Vec3& value) \
{ \
    if( ! getWorld() ) { \
        return ; \
    } \
    dVector3    vec ; \
    dOPE(vec, =, value) ; \
    if( m_functions.Get##NAME ) { \
        m_functions.Get##NAME(m_ODE_joint, vec) ; \
    } \
    dOPE(value, =, vec) ; \
}

READ_VEC3_IMPLEMENTATION(Axis1) ;
READ_VEC3_IMPLEMENTATION(Axis2) ;
READ_VEC3_IMPLEMENTATION(Axis3) ;
READ_VEC3_IMPLEMENTATION(Anchor1) ;
READ_VEC3_IMPLEMENTATION(Anchor2) ;

#undef READ_VEC3_IMPLEMENTATION
/* ....................................................................... */
/* ======================================================================= */
