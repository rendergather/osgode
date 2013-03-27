/*!
 * @file DefaultNearCallback.cpp
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
#include <osgODE/DefaultNearCallback>
#include <osgODE/OverlappingPair>
#include <osgODE/CollisionParameters>
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
DefaultNearCallback::DefaultNearCallback(void):
    m_contact_group( dJointGroupCreate(0) ),
    m_contacts(NULL),
    m_max_contact_num(0)
{
    setCollisionParameters( new CollisionParameters() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DefaultNearCallback::DefaultNearCallback(const DefaultNearCallback& other, const osg::CopyOp& copyop):
    NearCallback(other, copyop),
    m_contact_group( dJointGroupCreate(0) ),
    m_contacts(NULL),
    m_max_contact_num(0),
    m_collision_result( other.m_collision_result )
{
    setCollisionParameters( new CollisionParameters(*(other.m_collision_parameters)) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DefaultNearCallback::~DefaultNearCallback(void)
{
    dJointGroupEmpty(m_contact_group) ;
    dJointGroupDestroy(m_contact_group) ;

    delete m_contacts ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DefaultNearCallback::setCollisionParameters(CollisionParameters* collision_parameters)
{
    m_collision_parameters = collision_parameters ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
DefaultNearCallback::allocateContactArray(unsigned int N)
{
    PS_DBG2("osgODE::DefaultNearCallback::allocateContactArray(%p, N=%u)", this, N) ;


    delete m_contacts ;
    m_contacts = new dContact[N] ;

    if( m_contacts ) {
        m_max_contact_num = N ;
        return true ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DefaultNearCallback::reset(void)
{
    PS_DBG3("osgODE::DefaultNearCallback::reset(%p)", this) ;

    dJointGroupEmpty(m_contact_group) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
int
DefaultNearCallback::collide(OverlappingPair* op)
{
//     PS_DBG3("osgODE::DefaultNearCallback::collide(%p, op=%p)", this, op) ;


    entryPoint(op) ;

    osg::ref_ptr<CollisionParameters>   collision_parameters = computeCollisionParameters(op) ;

    return generateContacts(op, collision_parameters.get()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DefaultNearCallback*
DefaultNearCallback::asDefaultNearCallback(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DefaultNearCallback::addContact(dBodyID body1, dBodyID body2, dContact* contact)
{
    dWorldID    world = NULL ;


    switch(  (body1 != NULL) | (body2 != NULL) << 1  )
    {
        case 0:
            PS_DBG2( "osgODE::DefaultNearCallback::addContact(%p): body1 == body2 == NULL, skipping contact", this) ;
            return ;


        case 1:
            world = dBodyGetWorld(body1) ;
            break ;


        case 2:
            world = dBodyGetWorld(body2) ;
            break ;


        case 3:
            PS_ASSERT1( dBodyGetWorld(body1) == dBodyGetWorld(body2) ) ;
            world = dBodyGetWorld(body1) ;
            break ;


        default:
            PS_FATAL("Very bad things at %s:%u", __FILE__, __LINE__) ;
            PS_ASSERT1( false ) ;
        break ;
    }


    PS_ASSERT1( world != NULL ) ;


    dJointID    contact_joint = dJointCreateContact(world, m_contact_group, contact) ;
    dJointAttach(contact_joint, body1, body2) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DefaultNearCallback::entryPoint(OverlappingPair* op)
{
    PS_ASSERT1( op != NULL ) ;
    PS_ASSERT1( op->getCollidable1() != NULL  &&  op->getCollidable2() != NULL ) ;
    PS_ASSERT1( op->getCollidable1()->getWorld() != NULL ) ;
    PS_ASSERT1( op->getCollidable2()->getWorld() != NULL ) ;
    PS_ASSERT1( op->getCollidable1()->getWorld() == op->getCollidable2()->getWorld() ) ;
    PS_ASSERT1( op->getCollidable1()->getWorld()->asSpace() != NULL ) ;


    m_collision_result.resetInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CollisionParameters*
DefaultNearCallback::computeCollisionParameters( OverlappingPair* op )
{
    CollisionParameters*    collision_parameters1 = op->getCollidable1()->getCollisionParameters() ;
    CollisionParameters*    collision_parameters2 = op->getCollidable2()->getCollisionParameters() ;



    CollisionParameters*    collision_parameters = NULL ;



    switch( (int)(collision_parameters1 != NULL) | ((int)(collision_parameters2 != NULL) << 1) )
    {
        /* collision_parameters1 */
        case 1:
            collision_parameters = _mixCollisionParameters(collision_parameters1, m_collision_parameters.get()) ;
        break ;


        /* collision_parameters2 */
        case 2:
            collision_parameters = _mixCollisionParameters(m_collision_parameters.get(), collision_parameters2) ;
        break ;


        /* collision_parameters1 and collision_parameters2 */
        case 3:
            collision_parameters = _mixCollisionParameters(collision_parameters1, collision_parameters2) ;
        break ;


        default:
            collision_parameters = m_collision_parameters.get() ;
        break ;
    }



    return collision_parameters ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
int
DefaultNearCallback::generateContacts(OverlappingPair* op, CollisionParameters* collision_parameters)
{
    Collidable*     collidable1 = op->getCollidable1() ;
    Collidable*     collidable2 = op->getCollidable2() ;


    dBodyID         body1 = collidable1->getODEBody() ;
    dBodyID         body2 = collidable2->getODEBody() ;

    dGeomID         geom1 = collidable1->getODEGeom() ;
    dGeomID         geom2 = collidable2->getODEGeom() ;





    if( m_max_contact_num < m_collision_parameters->getMaxContactNum() ) {
        PS_ASSERT( allocateContactArray( m_collision_parameters->getMaxContactNum() ) ) ;
    }





    if( ! collision_parameters->getCollideConnected() ) {

        if( dAreConnectedExcluding(body1, body2, dJointTypeContact) ) {
            return 0 ;
        }
    }







    int contacts_num = dCollide(geom1, geom2, m_collision_parameters->getMaxContactNum(), &(m_contacts->geom), sizeof(dContact)) ;




    if(contacts_num > 0) {
        PS_DBG3("osgODE::DefaultNearCallback::generateContacts(%p, ...): colliding %p <=> %p", this, collidable1, collidable2) ;

        m_collision_result.setCollidable1(collidable1) ;
        m_collision_result.setCollidable2(collidable2) ;

        dContact*           contact = m_contacts ;
        dContact*           contact_end = &m_contacts[contacts_num] ;


        osg::Vec4   p = osg::Vec4(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2], 0.0) ;

        osg::Vec3   fdir = collidable2->getPointVelocity( p ) - collidable1->getPointVelocity( p ) ;
//         osg::Vec3   fdir = collidable2->getLinearVelocity() - collidable1->getLinearVelocity() ;


        unsigned int    collision_mode = collision_parameters->getMode() ;

        if( fdir.normalize() < 1.0e-5 ) {
            collision_mode &= ~dContactFDir1 ;
        }



        if( collision_parameters->getCollisionsEnabled() ) {

            while(contact != contact_end) {

                contact->surface.mode =         collision_mode ;
                contact->surface.mu =           collision_parameters->getMu() ;
                contact->surface.mu2 =          collision_parameters->getMu2() ;
                contact->surface.bounce =       collision_parameters->getBounce() ;
                contact->surface.bounce_vel =   collision_parameters->getBounceVel() ;
                contact->surface.soft_erp =     collision_parameters->getSoftERP() ;
                contact->surface.soft_cfm =     collision_parameters->getSoftCFM() ;
                contact->surface.motion1 =      collision_parameters->getMotion1() ;
                contact->surface.motion2 =      collision_parameters->getMotion2() ;
                contact->surface.motionN =      collision_parameters->getMotionN() ;
                contact->surface.slip1 =        collision_parameters->getSlip1() ;
                contact->surface.slip2 =        collision_parameters->getSlip2() ;



                dOPE(contact->fdir1, =, fdir) ;


                addContact( dBodyIsKinematic(body1) ? NULL : body1,
                            dBodyIsKinematic(body2) ? NULL : body2,
                            contact
                        ) ;




                // store collisions in CollisionResult
                m_collision_result.addContactInternal(

                                        osg::Vec3(  contact->geom.normal[0],
                                                    contact->geom.normal[1],
                                                    contact->geom.normal[2] ),

                                        osg::Vec3(  contact->geom.pos[0],
                                                    contact->geom.pos[1],
                                                    contact->geom.pos[2] ),

                                        contact->geom.depth                      ) ;




                contact++ ;
            }

        } else { // collision_parameters->getCollisionsEnabled()

            //
            // store collisions in CollisionResult
            //

            while(contact != contact_end) {

                m_collision_result.addContactInternal(

                                        osg::Vec3(  contact->geom.normal[0],
                                                    contact->geom.normal[1],
                                                    contact->geom.normal[2] ),

                                        osg::Vec3(  contact->geom.pos[0],
                                                    contact->geom.pos[1],
                                                    contact->geom.pos[2] ),

                                        contact->geom.depth                 ) ;


                contact++ ;
            }
        }
    }


    return contacts_num ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Se solo uno dei due ha il parametro abilitato, viene preso quello;
// se entrambi hanno il parametro abilitato, viene fatto (P1+P2)/2.
// Per quel che riguarda il campo m_collisions_enabled viene fatta una AND
// fra i due
#define MIX_PARAMETER(ODE_PARAM, PARAM, CP1, CP2, RESULT) \
{ \
    switch( (((CP1)->getMode() & (ODE_PARAM)) != 0) | ((((CP2)->getMode() & (ODE_PARAM)) != 0) << 1) ) { \
        case 0: \
        break ; \
\
        case 1: \
            (RESULT)->set##PARAM( (CP1)->get##PARAM() ) ; \
        break ; \
\
        case 2: \
            (RESULT)->set##PARAM( (CP2)->get##PARAM() ) ; \
        break ; \
\
        case 3: \
            (RESULT)->set##PARAM( ((CP1)->get##PARAM() + (CP2)->get##PARAM()) * 0.5 ) ; \
        break ; \
\
        default: \
            PS_BREAKPOINT() ; \
        break ; \
    } \
}
/* ....................................................................... */
CollisionParameters*
DefaultNearCallback::_mixCollisionParameters(CollisionParameters* cp1, CollisionParameters* cp2)
{
//     PS_DBG3("osgODE::DefaultNearCallback::_mixCollisionParameters(%p, cp1=%p, cp2=%p)", this, cp1, cp2) ;


    CollisionParameters*    result = new CollisionParameters() ;


    result->setMode( cp1->getMode() | cp2->getMode() ) ;
    result->setMu( (cp1->getMu() + cp2->getMu()) / 2.0 ) ;



    MIX_PARAMETER(dContactMu2, Mu2, cp1, cp2, result) ;
    MIX_PARAMETER(dContactBounce, Bounce, cp1, cp2, result) ;
    MIX_PARAMETER(dContactBounce, BounceVel, cp1, cp2, result) ;
    MIX_PARAMETER(dContactSoftERP, SoftERP, cp1, cp2, result) ;
    MIX_PARAMETER(dContactSoftCFM, SoftCFM, cp1, cp2, result) ;
    MIX_PARAMETER(dContactMotion1, Motion1, cp1, cp2, result) ;
    MIX_PARAMETER(dContactMotion2, Motion2, cp1, cp2, result) ;
    MIX_PARAMETER(dContactMotionN, MotionN, cp1, cp2, result) ;
    MIX_PARAMETER(dContactSlip1, Slip1, cp1, cp2, result) ;
    MIX_PARAMETER(dContactSlip2, Slip2, cp1, cp2, result) ;



    result->setCollideConnected( cp1->getCollideConnected() || cp2->getCollideConnected() ) ;

    result->setCollisionsEnabled( cp1->getCollisionsEnabled() && cp2->getCollisionsEnabled() ) ;


    return result ;
}
/* ....................................................................... */
#undef MIX_PARAMETER
/* ======================================================================= */
