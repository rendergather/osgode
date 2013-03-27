/*!
 * @file Box.cpp
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
#include <osgODE/Box>
#include <osgODE/Space>
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
Box::Box(void)
{
    m_ODE_geom = dCreateBox(NULL, REAL(1.0), REAL(1.0), REAL(1.0)) ;
    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Box::Box(const Box& other, const osg::CopyOp& copyop):
    Collidable(other, copyop)
{
    m_ODE_geom = dCreateBox(NULL, REAL(1.0), REAL(1.0), REAL(1.0)) ;

    _cloneODEBox(other.m_ODE_geom, m_ODE_geom) ;

    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;


    setCollidableOffset(other.getCollidableOffset()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Box::~Box(void)
{
    dGeomDestroy(m_ODE_geom) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Box::setSize(const osg::Vec3& size)
{
    PS_DBG2("osgODE::Box::setSize(%p, size=[%lf, %lf, %lf])", this, size[0], size[1], size[2]) ;

    dGeomBoxSetLengths(m_ODE_geom, size[0], size[1], size[2]) ;


    // We need this to recompute the inertia tensors
    setMass( getMass() ) ;


    if( getUpdateInteractingSphere() ) {
        _updateInteractingSphere() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Vec3
Box::getSize(void) const
{
    dVector3    size ;
    dGeomBoxGetLengths(m_ODE_geom, size) ;

    return osg::Vec3(size[0], size[1], size[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Box::_cloneODEBox(dGeomID src, dGeomID dst)
{
    PS_DBG2("osgODE::Box::_cloneODEBox(src=%p, dest=%p)", src, dst) ;

    dVector3    l ;
    dGeomBoxGetLengths(src, l) ;
    dGeomBoxSetLengths(dst, l[0], l[1], l[2]) ;

    Collidable::_cloneODEGeom(src, dst) ;
}
/* ....................................................................... */
/* ======================================================================= */
