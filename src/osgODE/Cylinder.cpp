/*!
 * @file Cylinder.cpp
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
#include <osgODE/Cylinder>
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
Cylinder::Cylinder(void)
{
    m_ODE_geom = dCreateCylinder(NULL, REAL(0.5), REAL(1.0)) ;
    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Cylinder::Cylinder(const Cylinder& other, const osg::CopyOp& copyop):
    Collidable(other, copyop)
{
    m_ODE_geom = dCreateCylinder(NULL, REAL(0.5), REAL(1.0)) ;

    _cloneODECylinder(other.m_ODE_geom, m_ODE_geom) ;

    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;


    setCollidableOffset(other.getCollidableOffset()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Cylinder::~Cylinder(void)
{
    dGeomDestroy(m_ODE_geom) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Cylinder::setSize(const osg::Vec3& size)
{
    PS_DBG2("osgODE::Cylinder::setSize(%p, size=[%lf, %lf, %lf])", this, size[0], size[1], size[2]) ;

    dGeomCylinderSetParams(m_ODE_geom, (size[0] + size[1]) * 0.25, size[2]) ;


    setMass( getMass() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Vec3
Cylinder::getSize(void) const
{
    dReal   r, l ;

    dGeomCylinderGetParams(m_ODE_geom, &r, &l) ;

    dReal   d = r * REAL(2.0) ;

    return osg::Vec3(d, d, l) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Cylinder::_cloneODECylinder(dGeomID src, dGeomID dst)
{
    PS_DBG2("osgODE::Cylinder::_cloneODECylinder(src=%p, dest=%p)", src, dst) ;

    dReal   r, l ;
    dGeomCylinderGetParams(src, &r, &l) ;
    dGeomCylinderSetParams(dst, r, l) ;

    Collidable::_cloneODEGeom(src, dst) ;
}
/* ....................................................................... */
/* ======================================================================= */
