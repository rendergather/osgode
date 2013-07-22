/*!
 * @file NoGravityVolume.cpp
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
#include <osgODE/NoGravityVolume>
#include <osgODE/NoGravityVolumeCollisionCallback>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
NoGravityVolume::NoGravityVolume(void)
{
    CollisionParameters*    cp = new CollisionParameters() ;
    cp->setCollisionsEnabled( false ) ;

    this->Sphere::setCollisionParameters( cp ) ;
    this->Sphere::setGravityMode( false ) ;
    this->Sphere::setKinematic( true ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NoGravityVolume::NoGravityVolume(const NoGravityVolume& other, const osg::CopyOp& copyop):
    Sphere      ( other, copyop )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NoGravityVolume::~NoGravityVolume(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NoGravityVolume::postUpdate(double step_size)
{
    Collidables::iterator   itr = m_collidables.begin() ;
    Collidables::iterator   itr_end = m_collidables.end() ;

    while( itr != itr_end ) {
        (*itr++)->setGravityMode( true ) ;
    }


    m_collidables.clear() ;


    this->Sphere::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NoGravityVolume::setupCallbacks(void)
{
    addCollisionCallback( new NoGravityVolumeCollisionCallback() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NoGravityVolume*
NoGravityVolume::asNoGravityVolume(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
