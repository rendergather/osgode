/*!
 * @file CollisionPredictor.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/CollisionPredictor>
#include <osgODE/Collidable>
#include <osgODE/Space>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
CollisionPredictor::CollisionPredictor(void):
    m_ray_cast_result( new AllRayCastResult() )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CollisionPredictor::CollisionPredictor(const CollisionPredictor& other, const osg::CopyOp& copyop):
    ODECallback(other, copyop)
{
    if( copyop.getCopyFlags() && osg::CopyOp::DEEP_COPY_CALLBACKS ) {
        m_ray_cast_result = new AllRayCastResult( *other.m_ray_cast_result ) ;
    } else {
        m_ray_cast_result = other.m_ray_cast_result ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CollisionPredictor::~CollisionPredictor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CollisionPredictor::operator()(ODEObject* object)
{

    Collidable* body = object->asCollidable() ;
    PS_ASSERT1( body != NULL) ;



    World*      world = body->getWorld() ;
    PS_ASSERT1( world != NULL ) ;


    Space*      space = world->asSpace() ;
    PS_ASSERT1( space != NULL ) ;


    const osg::Vec3 size = body->getSize() ;

    const double    size_min = osg::minimum( size.x(), osg::minimum(size.y(), size.z()) ) ;



    // compute the ray
    const osg::Vec3 from = body->getPosition() ;
    const osg::Vec3 to = from + body->getLinearVelocity() * space->getCurrentStepSize() ;


    osg::Vec3   direction = to - from ;


    // Check wheter it's too slow to miss collisions
    if( direction.normalize() < size_min * 0.45 ) {
        traverse( object ) ;
        return ;
    }



    //
    // Ray cast
    //

    m_ray_cast_result->reset() ;

    space->rayCast( from, to, m_ray_cast_result, 4, false, false, true ) ;

    m_ray_cast_result->sortByDistance() ;


    const AllRayCastResult::CollisionList&  collisions = m_ray_cast_result->getCollisionList() ;






    // no collision: return
    if( collisions.size() == 0 ) {
        traverse( object ) ;
        return ;
    }


    // the collision is inside the box and will be detected by dSpaceCollide
    if( collisions[0].mCollidable != body ) {
        traverse( object ) ;
        return ;
    }



    if( collisions.size() >= 2 ) {

        // move the body to place where its box contains the
        // collision point

        const osg::Vec3 delta = direction * collisions[0].mDistance * 0.5 ;


        body->setPosition( collisions[1].mPosition - delta ) ;
    }




    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
