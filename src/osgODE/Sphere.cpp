/*!
 * @file Sphere.cpp
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
#include <osgODE/Sphere>
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
Sphere::Sphere(void)
{
    m_ODE_geom = dCreateSphere(NULL, REAL(1.0)) ;

    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Sphere::Sphere(const Sphere& other, const osg::CopyOp& copyop):
    Collidable(other, copyop)
{
    m_ODE_geom = dCreateSphere(NULL, REAL(1.0)) ;

    _cloneODESphere(other.m_ODE_geom, m_ODE_geom) ;

    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;


    setCollidableOffset(other.getCollidableOffset()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Sphere::~Sphere(void)
{
    dGeomDestroy(m_ODE_geom) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Sphere::setSize(const osg::Vec3& size)
{
    PS_DBG2("osgODE::Sphere::setSize(%p, size=[%lf, %lf, %lf])", this, size[0], size[1], size[2]) ;

    dGeomSphereSetRadius(m_ODE_geom, (size[0] + size[1] + size[2]) / 6.0) ;


    // Recalculate the inertia tensors
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
Sphere::getSize(void) const
{
    const double        diameter = dGeomSphereGetRadius(m_ODE_geom) * 2.0 ;

    return osg::Vec3(diameter, diameter, diameter) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Sphere::_cloneODESphere(dGeomID src, dGeomID dst)
{
    PS_DBG2("osgODE::Sphere::_cloneODESphere(src=%p, dest=%p)", src, dst) ;


    dGeomSphereSetRadius(dst, dGeomSphereGetRadius(src)) ;


    Collidable::_cloneODEGeom(src, dst) ;
}
/* ....................................................................... */
/* ======================================================================= */
