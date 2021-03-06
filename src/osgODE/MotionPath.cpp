/*!
 * @file MotionPath.cpp
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
#include <osgODE/MotionPath>
#include <osgODE/RigidBody>
#include <osgODE/World>
#include <osgODE/FixedJoint>
#include <osgODE/CommonWorldOperations>
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
MotionPath::MotionPath(void):
    m_time              ( -1.0 ),
    m_status            ( STOP ),
    m_remove_when_done  ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotionPath::MotionPath(const MotionPath& other, const osg::CopyOp& copyop):
    ODECallback         ( other, copyop ),
    m_position          ( osg::clone(other.m_position.get(), copyop) ),
    m_quaternion        ( osg::clone(other.m_quaternion.get(), copyop) ),
    m_time              ( other.m_time ),
    m_status            ( other.m_status ),
    m_remove_when_done  ( other.m_remove_when_done )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
MotionPath::~MotionPath(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
MotionPath::operator()(ODEObject* object)
{
    RigidBody*          body = object->asRigidBody() ;
    FixedJoint*         fixed_joint = object->asFixedJoint() ;

    PS_ASSERT1( body  ||  fixed_joint ) ;


    switch( m_status )
    {
        case STOP:
            m_time = -1.0 ;
        break ;


        case PLAY:
        {

            m_time = osg::maximum(m_time, (ooReal)0.0) + body->getWorld()->getCurrentStepSize() ;

            osg::Matrix m ;

            if( m_quaternion ) {
                m.makeRotate( m_quaternion->interpolate( m_time ) ) ;
            }

            if( m_position ) {
                m = m * osg::Matrix::translate( m_position->interpolate( m_time ) ) ;
            }



            if( body ) {
                body->setMatrix( m ) ;
                body->setLinearVelocity( osg::Vec3() ) ;
                body->setAngularVelocity( osg::Vec3() ) ;
            }
            else if( fixed_joint ) {
                fixed_joint->setMatrix( m ) ;
            }



            if( m_remove_when_done ) {

                ooReal  max_duration = 0.0 ;

                if( m_quaternion ) {
                    max_duration = osg::maximum( max_duration, m_quaternion->getDuration() ) ;

                    if( m_quaternion->getLooping() ) {
                        max_duration = FLT_MAX ;
                    }
                }

                if( m_position ) {
                    max_duration = osg::maximum( max_duration, m_position->getDuration() ) ;

                    if( m_position->getLooping() ) {
                        max_duration = FLT_MAX ;
                    }
                }


                if( m_time > max_duration ) {

                    ODEObject*  obj = NULL ;

                    if      ( body )                    obj = body ;
                    else if ( fixed_joint )             obj = fixed_joint ;



                    World*  world = obj->getWorld() ;

                    PS_ASSERT1( world ) ;


                    world->addOperation( new ModifyCallbackOperation(obj, this, &ODEObject::removeUpdateCallback) ) ;
                    world->addOperation( new ModifyCallbackOperation(obj, this, &ODEObject::removePostUpdateCallback) ) ;


                }
            }
        }
        break ;


        case PAUSE:
        break ;
    } ;


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
