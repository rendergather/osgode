/*!
 * @file ShockWaveCollisionCallback.cpp
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/ShockWaveCollisionCallback>
#include <osgODE/ShockWave>
#include <osgODE/DefaultNearCallback>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
ShockWaveCollisionCallback::ShockWaveCollisionCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ShockWaveCollisionCallback::ShockWaveCollisionCallback(const ShockWaveCollisionCallback& other, const osg::CopyOp& copyop):
    CollisionCallback(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ShockWaveCollisionCallback::~ShockWaveCollisionCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ShockWaveCollisionCallback::operator()(Collidable* owner, Collidable* other, NearCallback* near_callback)
{

    if( other->asShockWave() ) {

        this->CollisionCallback::traverse(owner, other, near_callback) ;
        return ;
    }


    ShockWave*  shock_wave = owner->asShockWave() ;
    PS_ASSERT1( shock_wave != NULL ) ;


    DefaultNearCallback*   default_near_callback = near_callback->asDefaultNearCallback() ;
    PS_ASSERT1( default_near_callback != NULL ) ;



    const osg::Vec3 shock_direction = shock_wave->getQuaternion() * shock_wave->getDirection() ;
    const double    cos_angle = cos( shock_wave->getAngle() ) ;



    const DefaultNearCallback::CollisionResult& collision_result = default_near_callback->getCollisionResult() ;



    const double        current_force = shock_wave->getCurrentForce() ;
    const unsigned int  num_contacts = collision_result.getNumContacts() ;

    PS_ASSERT1( num_contacts > 0 ) ;

    const DefaultNearCallback::CollisionResult::Contacts&   contacts = collision_result.getContacts() ;

    osg::Vec3   position ;

    for(unsigned int i=0; i<num_contacts; i++) {
        position += contacts[i] ;
    }

    position /= num_contacts ;




    osg::Vec3       force = (position - shock_wave->getPosition()) ;
    const double    distance_from_peak = shock_wave->getCurrentRadius() - force.normalize() ;



    force *= (force * shock_direction) >= cos_angle ;


    force *= current_force * ( distance_from_peak >= shock_wave->getWaveSize() ) ;


    if( force.length() >= shock_wave->getThreshold() ) {
        other->addForce( force, position, false, false ) ;

        if( ! other->getBodyEnabled() ) {
            other->setBodyEnabled(true) ;
        }
    }




    this->CollisionCallback::traverse(owner, other, near_callback) ;
}
/* ....................................................................... */
/* ======================================================================= */
