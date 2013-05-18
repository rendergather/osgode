/*!
 * @file Character.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Character>
#include <osgODE/FixedJoint>
#include <osgODE/CommonRayCastResults>
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
Character::Character(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Character::Character(const Character& other, const osg::CopyOp& copyop):
    CharacterBase(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Character::~Character(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Character::attach(  RigidBody*  body,
                    double      erp,
                    double      cfm
                 )
{

    if( ! body ) {
        PS_WARN("osgODE::Character::attach(%p): null body", this) ;
        return false ;
    }


    PS_DBG("osgODE::Character::attach(%p, body=%p)", this, body) ;


    Joint*  joint = new FixedJoint() ;


    joint->setBody1( this->CharacterBase::getBody() ) ;
    joint->setBody2( body ) ;


    joint->setParam( dParamERP, erp ) ;
    joint->setParam( dParamCFM, cfm ) ;




    // It is already in the world
//     this->Container::addObject( body ) ;


    this->Container::addObject( joint ) ;

    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Character::detach(  RigidBody* body )
{
    if( ! body ) {
        PS_WARN("osgODE::Character::detach(%p): null body", this) ;
        return false ;
    }


    PS_DBG("osgODE::Character::detach(%p, body=%p)", this, body) ;


    RigidBody::JointList    joint_list( body->getJointList() ) ;



    for( RigidBody::JointList::iterator itr = joint_list.begin() ;
         itr != joint_list.end() ;
         itr++ )

    {
        removeObject(  getObjectIDX( itr->get() )    ) ;
    }





//     removeObject( getObjectIDX(body) ) ;

    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBody*
Character::touch( double max_distance )
{

    World*  world = getWorld() ;
    PS_ASSERT1( world != NULL ) ;


    Space*  space = world->asSpace() ;

    if( ! space ) {
        return NULL ;
    }





    RigidBody*  body = getBody() ;


    if( NULL == body ) {
        PS_WARN("osgODE::Character::touch(%p): no rigid body", this) ;
        return NULL ;
    }






    const osg::Vec3     from = body->getPosition() ;

    const osg::Vec3     to = from + body->getQuaternion() * getFrontVersor() * max_distance ;




    osg::ref_ptr<NearestNotMeRayCastResult> result = new NearestNotMeRayCastResult( body->asCollidable() ) ;


    space->rayCast( from,
                    to,
                    result.get(),
                    8,
                    false,      // first contact
                    true,       // back face cull
                    true        // closest hit
           ) ;



    return result->getCollidable() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBody*
Character::touch( double max_distance, osg::Vec3& position, osg::Vec3& normal )
{

    World*  world = getWorld() ;
    PS_ASSERT1( world != NULL ) ;


    Space*  space = world->asSpace() ;

    if( ! space ) {
        return NULL ;
    }





    RigidBody*  body = getBody() ;


    if( NULL == body ) {
        PS_WARN("osgODE::Character::touch(%p): no rigid body", this) ;
        return NULL ;
    }






    const osg::Vec3     from = body->getPosition() ;

    const osg::Vec3     to = from + body->getQuaternion() * getFrontVersor() * max_distance ;




    osg::ref_ptr<NearestNotMeRayCastResult> result = new NearestNotMeRayCastResult( body->asCollidable() ) ;


    space->rayCast( from,
                    to,
                    result.get(),
                    8,
                    false,      // first contact
                    true,       // back face cull
                    true        // closest hit
           ) ;



    position.set( result->getPosition() ) ;
    normal.set( result->getNormal() ) ;



    return result->getCollidable() ;
}
/* ....................................................................... */
/* ======================================================================= */
