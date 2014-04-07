/*!
 * @file Collidable.cpp
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
#include <osgODE/Collidable>
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
Collidable::Collidable(void):
    m_last_collision_frame  ( 0 )
{
    m_ODE_geom = NULL ;

    m_matrix_offset.makeIdentity() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Collidable::Collidable(const Collidable& other, const osg::CopyOp& copyop):
    RigidBody               ( other, copyop ),
    m_collision_parameters  ( other.m_collision_parameters ),
    m_collision_callback    ( other.m_collision_callback ),
    m_last_collision_frame  ( other.m_last_collision_frame )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Collidable::~Collidable(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Collidable::setCollidableOffset(const osg::Matrix& offset)
{

    dGeomSetOffsetPosition(m_ODE_geom, offset(3,0), offset(3,1), offset(3,2)) ;


    // Transpose the matrix

    dMatrix3    R ;

    R[0]  = offset(0,0) ; R[1]  = offset(1,0) ; R[2]  = offset(2,0) ; R[3]  = 0.0 ;
    R[4]  = offset(0,1) ; R[5]  = offset(1,1) ; R[6]  = offset(2,1) ; R[7]  = 0.0 ;
    R[8]  = offset(0,2) ; R[9]  = offset(1,2) ; R[10] = offset(2,2) ; R[11] = 0.0 ;



    dGeomSetOffsetRotation(m_ODE_geom, R) ;



    m_matrix_offset = offset ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Collidable::addToWorldInternal(World* world)
{
    PS_DBG2("osgODE::Collidable::addToWorldInternal(%p, world=%p)", this, world) ;



    /*
     * During the RigidBody::addToWorldInternal we recreate the dBodyID, so we
     * must call it before everything else in order to correctly use the
     * dGeomSetBody function
     */
    bool    rb = RigidBody::addToWorldInternal(world) ;

    /*!
     * @todo    check the RigidBody::addToWorldInternal return value
     */

    (void) rb ;


    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    // Recreate the geometry offset - indispensable
    setCollidableOffset(m_matrix_offset) ;



    Space*  space = world->asSpace() ;

    if( space ) {

        if( dGeomGetSpace(m_ODE_geom) != space->getODESpace() ) {
            dSpaceAdd(space->getODESpace(), m_ODE_geom) ;
        }

    } else {
        PS_WARN("osgODE::Collidable::addToWorldInternal(%p, ...): %p not a Space", this, world) ;
    }


    return rb ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Collidable::removeFromWorldInternal(World* world)
{
    PS_DBG2("osgODE::Collidable::removeFromWorldInternal(%p, world=%p)", this, world) ;




    bool    rb = RigidBody::removeFromWorldInternal(world) ;

    /*!
     * @todo    need to check the RigidBody::removeFromWorldInternal return value
     */

    (void) rb ;



    Space*  space = world->asSpace() ;


    if( space ) {
        if( dGeomGetSpace(m_ODE_geom) == space->getODESpace() ) {
            dSpaceRemove(space->getODESpace(), m_ODE_geom) ;
        }

    } else {
        PS_WARN("osgODE::Collidable::removeFromWorldInternal(%p, ...): %p not a Space", this, world) ;
    }


    return rb ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Collidable::setMass(double mass)
{
    this->RigidBody::setMass(mass, getSize(), dGeomGetClass(m_ODE_geom)) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Collidable::setDensity(double density)
{
    this->RigidBody::setDensity(density, getSize(), dGeomGetClass(m_ODE_geom)) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::BoundingBox
Collidable::getAABB(void) const
{
    dReal   aabb[6] ;

    dGeomGetAABB(m_ODE_geom, aabb) ;

    return osg::BoundingBox(aabb[0], aabb[2], aabb[4], aabb[1], aabb[3], aabb[5]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Collidable*
Collidable::asCollidable(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Collidable::isColliding(void) const
{
    const World*    world = getWorld() ;

    if( world ) {
        return m_last_collision_frame == world->getCurrentFrame() ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Collidable::_cloneODEGeom(dGeomID src, dGeomID dst)
{
    PS_DBG2("osgODE::Collidable::_cloneODEGeom(src=%p, dest=%p)", src, dst) ;


    // position and orientation
    {
        /*
         * We don't need this because we invoke the setCollidableOffset in
         * addToWorldInternal
         */

#if 0
        const dReal*    pos = dGeomGetPosition(src) ;
        dGeomSetPosition(dst, pos[0], pos[1], pos[2]) ;
        dGeomSetRotation(dst, dGeomGetRotation(src)) ;

        const dReal*    off_pos = dGeomGetOffsetPosition(src) ;
        dGeomSetOffsetPosition(dst, off_pos[0], off_pos[1], off_pos[2]) ;
        dGeomSetOffsetRotation(dst, dGeomGetOffsetRotation(src)) ;
#endif
    }


    // collision
    {
        dGeomSetCategoryBits(   dst,    dGeomGetCategoryBits(   src)) ;
        dGeomSetCollideBits(    dst,    dGeomGetCollideBits(    src)) ;
    }


    {
        if( dGeomIsEnabled(src) ) {
            dGeomEnable(dst) ;
        } else {
            dGeomDisable(dst) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */
