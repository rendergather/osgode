/*!
 * @file RigidBody.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2013 by Rocco Martino                            *
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
#include <osgODE/RigidBody>
#include <osgODE/World>
#include <osgODE/StaticWorld>
#include <osgODE/Joint>
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
RigidBody::RigidBody(void):
    m_mass_negative(false),
    m_update_interacting_sphere(false)
{
    dWorldID    static_world = StaticWorld::instance()->getODEWorld() ;

    // The body is initially created in the static world
    m_ODE_body = dBodyCreate(static_world) ;

    dBodySetData(m_ODE_body, this) ;


    // Some default parameters...
    setMaxAngularSpeed(FLT_MAX) ;
    setLinearDamping(0.0) ;
    setAngularDamping(0.0) ;
    setLinearDampingThreshold(0.0) ;
    setAngularDampingThreshold(0.0) ;
    setAutoDisableLinearThreshold(0.01) ;
    setAutoDisableAngularThreshold(0.01) ;
    setAutoDisableFlag(false) ;
    setAutoDisableSteps(60) ;
    setAutoDisableTime(1.0) ;
    setAutoDisableAverageSamplesCount(5) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBody::RigidBody(const RigidBody& other, const osg::CopyOp& copyop):
    Transformable(other, copyop),
    m_mass_negative( other.m_mass_negative ),
    m_update_interacting_sphere(other.m_update_interacting_sphere),
    m_old_linear_velocity( other.m_old_linear_velocity ),
    m_linear_acceleration( other.m_linear_acceleration ),
    m_old_angular_velocity( other.m_old_angular_velocity ),
    m_angular_acceleration( other.m_angular_acceleration )
{
    dWorldID    static_world = StaticWorld::instance()->getODEWorld() ;
    m_ODE_body = dBodyCreate(static_world) ;


    _cloneODEBody(other.m_ODE_body, m_ODE_body) ;


    dBodySetData(m_ODE_body, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBody::~RigidBody(void)
{
    if(m_ODE_body) {
        dBodyDestroy(m_ODE_body) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::setMass(double mass)
{
    setMass(mass, osg::Vec3(1.0, 1.0, 1.0), dSphereClass) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::setMass(double mass, const osg::Vec3& size, int type_class)
{
    PS_DBG2(    "osgODE::RigidBody::setMass(%p, mass=%f, size=[%f, %f, %f], class=%d)",
                this, mass, size.x(), size.y(), size.z(), type_class ) ;



    // check for negative mass
    if( mass <= 0.0 ) {

        mass = fabs( mass ) ;

        m_mass_negative = true ;

        this->setGravityMode(false) ;



    } else {

        m_mass_negative = false ;

    }






    // The kinematic bodies have mass->invMass = 0, the dBodySetMass restore
    // this value. To preserve the body state I store the kinematic flag here
    // and restore it at the end of this function
    bool    kinematic_flag = this->getKinematic() ;



    dMass   m ;

    switch(type_class)
    {

        case dBoxClass:

            dMassSetBoxTotal(&m, mass, size.x(), size.y(), size.z()) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dCylinderClass:

            dMassSetCylinderTotal(&m, mass, 3, (size.x() + size.y()) / 4.0, size.z()) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dSphereClass:

            dMassSetSphereTotal(&m, mass, (size.x() + size.y() + size.z()) / 6.0) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dCapsuleClass:

            dMassSetCapsuleTotal(&m, mass, 3, (size.x() + size.y()) / 4.0, size.z() - (size.x() + size.y()) / 2.0) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        default:
            PS_FATAL("osgODE::RigidBody::setMass(%p, ...): invalid type (%u)", this, type_class) ;
        break ;
    }



    // Restore the kinematic flag
    this->setKinematic( kinematic_flag ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::setDensity(double density)
{
    // Assume a unit sphere
    setDensity(density, osg::Vec3(1.0, 1.0, 1.0), dSphereClass) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::setDensity(double density, const osg::Vec3& size, int type_class)
{
    PS_DBG2(    "osgODE::RigidBody::setDensity(%p, density=%f, size=[%f, %f, %f], class=%d)",
                this, density, size.x(), size.y(), size.z(), type_class ) ;



    // check for negative mass
    if( density <= 0.0 ) {

        density = fabs( density ) ;

        m_mass_negative = true ;

        this->setGravityMode(false) ;


    } else {

        m_mass_negative = false ;
    }



    // See setMass(mass, size, type_class) above
    bool    kinematic_flag = this->getKinematic() ;



    dMass   m ;

    switch(type_class)
    {

        case dBoxClass:

            dMassSetBox(&m, density, size.x(), size.y(), size.z()) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dCylinderClass:

            dMassSetCylinder(&m, density, 3, (size.x() + size.y()) / 4.0, size.z()) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dSphereClass:

            dMassSetSphere(&m, density, (size.x() + size.y() + size.z()) / 6.0) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        case dCapsuleClass:

            dMassSetCapsule(&m, density, 3, (size.x() + size.y()) / 4.0, size.z() - (size.x() + size.y()) / 2.0) ;
            dBodySetMass(m_ODE_body, &m) ;

        break ;




        default:
            PS_FATAL("osgODE::RigidBody::setDensity(%p, ...): invalid type (%u)", this, type_class) ;
        break ;
    }



    // Restore the kinematic flag
    this->setKinematic( kinematic_flag ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
osgODE::RigidBody::addForce(const osg::Vec3& f, const osg::Vec3& p, bool fl, bool pl)
{
    const int  bit_mask = ( (int)(pl) << 1 )  |  (int)(fl) ;

    switch( bit_mask )
    {
        case 0:
            // force_local=false, point_local=false
            dBodyAddForceAtPos(m_ODE_body,
                                    f.x(), f.y(), f.z(),
                                    p.x(), p.y(), p.z()) ;
        break ;



        case 1:
            // force_local=true, point_local=false
            dBodyAddRelForceAtPos(m_ODE_body,
                                    f.x(), f.y(), f.z(),
                                    p.x(), p.y(), p.z()) ;
        break ;



        case 2:
            // force_local=false, point_local=true
            dBodyAddForceAtRelPos(m_ODE_body,
                                    f.x(), f.y(), f.z(),
                                    p.x(), p.y(), p.z()) ;
        break ;



        case 3:
            // force_local=true, point_local=true
            dBodyAddRelForceAtRelPos(m_ODE_body,
                                        f.x(), f.y(), f.z(),
                                        p.x(), p.y(), p.z()) ;
        break ;



        default:
            PS_ASSERT( false ) ;
        break ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
osgODE::RigidBody::addTorque(const osg::Vec3& t, bool local)
{
    if( local ) {

        dBodyAddRelTorque(m_ODE_body, t.x(), t.y(), t.z()) ;


    } else {

        dBodyAddTorque(m_ODE_body, t.x(), t.y(), t.z()) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::BoundingBox
RigidBody::getAABB(void) const
{
    const osg::Vec3 center = getPosition() ;

    return osg::BoundingBox(center, center) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
RigidBody::addToWorldInternal(World* world)
{
    PS_DBG2("osgODE::RigidBody::addToWorldInternal(%p, world=%p)", this, world) ;

    if( dBodyGetWorld(m_ODE_body) != world->getODEWorld() ) {


        /*
         * [1] clone the body
         */

        dBodyID     b = dBodyCreate( world->getODEWorld() ) ;

        _cloneODEBody(m_ODE_body, b) ;




        /*
         * [2] Destroy the old body and replace it with the new one
         */
        dBodyDestroy(m_ODE_body) ;

        m_ODE_body = b ;


        // Store the instance pointer
        dBodySetData(m_ODE_body, this) ;



        /* In the Joint;;checkBodies method (invoked by _notifyJoints) a body
         * is valid if it has non-NULL world, but the world of this body,
         * although valid, has not been setted yet. I provide: */
        setWorldInternal(world) ;



        /*
         * [3] Tell the joints that this body is now part of the simulation
         */
        _notifyJoints() ;



        PS_DBG3("osgODE::RigidBody::addToWorldInternal(%p): ODE body: %p", this, m_ODE_body) ;



        return true ;
    }


    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
RigidBody::removeFromWorldInternal(World* world)
{
    PS_DBG2("osgODE::RigidBody::removeFromWorldInternal(%p, world=%p)", this, world) ;

    if( dBodyGetWorld(m_ODE_body) == world->getODEWorld() ) {


        /*
         * [1] clone the body
         */
        dBodyID     b = dBodyCreate( StaticWorld::instance()->getODEWorld() ) ;

        _cloneODEBody(m_ODE_body, b) ;



        /*
         * [2] Destroy the old body and replace it with the new one
         */
        dBodyDestroy(m_ODE_body) ;

        m_ODE_body = b ;


        // This is not really useful
        dBodySetData(m_ODE_body, this) ;



        // as in addToWorldInternal
        setWorldInternal(NULL) ;



        /*
         * [3] Tell the joints that this body is no longer part of the simulation
         */
        _notifyJoints() ;



        return true ;
    }


    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::update(double step_size)
{
    World*  world = getWorld() ;

    PS_ASSERT1( world != NULL ) ;


    if( m_update_interacting_sphere ) {
        _updateInteractingSphere() ;
    }


    if( m_mass_negative ) {
        osg::Vec3   F = world->getGravity() * this->getMass() ;
        this->addForce( F ) ;
    }


    this->Transformable::update(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::postUpdate(double step_size)
{
    updateTransformInternal() ;

    _computeAcceleration(step_size) ;

    this->Transformable::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::updateTransformInternal(void)
{
    const dReal*    p = dBodyGetPosition(m_ODE_body) ;
    const dReal*    r = dBodyGetRotation(m_ODE_body) ;


    osg::Matrix     m(  r[0] , r[4] , r[8] , 0.0,
                        r[1] , r[5] , r[9] , 0.0,
                        r[2] , r[6] , r[10], 0.0,
                        p[0] , p[1] , p[2] , 1.0 ) ;



    this->getMatrixTransform()->setMatrix(m) ;
    this->getMatrixTransform()->dirtyBound() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::_notifyJoints(void)
{
    unsigned int        size = m_joints.size() ;

    for(unsigned int i=0; i<size; i++) {

        Joint*  j = m_joints[i].get() ;

        if( j ) {
            j->checkBodiesInternal() ;

        } else {
            // eliminate the destroyed joints from the list
            m_joints[i--] = m_joints[--size] ;
        }
    }

    m_joints.resize(size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::_updateInteractingSphere(void)
{
    const osg::BoundingSphere&  bs = getInteractingSphere() ;

    setInteractingSphere(getPosition(), bs.radius()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::_computeAcceleration(double step_size)
{
    osg::Vec3   current_linear_velocity = getLinearVelocity() ;
    osg::Vec3   current_angular_velocity = getAngularVelocity() ;

    m_linear_acceleration = (current_linear_velocity - m_old_linear_velocity) / step_size ;
    m_angular_acceleration = (current_angular_velocity - m_old_angular_velocity) / step_size ;

    m_old_linear_velocity = current_linear_velocity ;
    m_old_angular_velocity = current_angular_velocity ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RigidBody*
RigidBody::asRigidBody(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RigidBody::_cloneODEBody(dBodyID src, dBodyID dst)
{
    PS_DBG2("osgODE::RigidBody::_cloneODEBody(src=%p, dest=%p)", src, dst) ;

    // position and orientation
    {
        const dReal*    pos = dBodyGetPosition(src) ;
        const dReal*    rot = dBodyGetRotation(src) ;
        const dReal*    l_vel = dBodyGetLinearVel(src) ;
        const dReal*    a_vel = dBodyGetAngularVel(src) ;

        dBodySetPosition(dst, pos[0], pos[1], pos[2]) ;
        dBodySetRotation(dst, rot) ;
        dBodySetLinearVel(dst, l_vel[0], l_vel[1], l_vel[2]) ;
        dBodySetAngularVel(dst, a_vel[0], a_vel[1], a_vel[2]) ;
    }


    // mass and force
    {
        dMass   m ;
        dBodyGetMass(src, &m) ;
        dBodySetMass(dst, &m) ;


        const dReal*    force = dBodyGetForce(src) ;
        const dReal*    torque = dBodyGetTorque(src) ;

        dBodySetForce(dst, force[0], force[1], force[2]) ;
        dBodySetTorque(dst, torque[0], torque[1], torque[2]) ;
    }


    // kinematic state
    {
        if( dBodyIsKinematic(src) ) {
            dBodySetKinematic(dst) ;
        } else {
            dBodySetDynamic(dst) ;
        }
    }


    // automatic enabling and disabling
    {
        dBodySetAutoDisableFlag(                dst, dBodyGetAutoDisableFlag(                   src)) ;
        dBodySetAutoDisableLinearThreshold(     dst, dBodyGetAutoDisableLinearThreshold(        src)) ;
        dBodySetAutoDisableAngularThreshold(    dst, dBodyGetAutoDisableAngularThreshold(       src)) ;
        dBodySetAutoDisableSteps(               dst, dBodyGetAutoDisableSteps(                  src)) ;
        dBodySetAutoDisableTime(                dst, dBodyGetAutoDisableTime(                   src)) ;
        dBodySetAutoDisableAverageSamplesCount( dst, dBodyGetAutoDisableAverageSamplesCount(    src)) ;
    }


    // damping
    {
        dBodySetLinearDamping(              dst,    dBodyGetLinearDamping(              src)) ;
        dBodySetAngularDamping(             dst,    dBodyGetAngularDamping(             src)) ;
        dBodySetLinearDampingThreshold(     dst,    dBodyGetLinearDampingThreshold(     src)) ;
        dBodySetAngularDampingThreshold(    dst,    dBodyGetAngularDampingThreshold(    src)) ;
        dBodySetMaxAngularSpeed(            dst,    dBodyGetMaxAngularSpeed(            src)) ;
    }


    // miscellaneous
    {
        dBodySetData(dst, dBodyGetData(src)) ;

        dBodySetFiniteRotationMode(dst, dBodyGetFiniteRotationMode(src)) ;

        dVector3    fra ;
        dBodyGetFiniteRotationAxis(src, fra) ;
        dBodySetFiniteRotationAxis(dst, fra[0], fra[1], fra[2]) ;

        dBodySetGravityMode(dst, dBodyGetGravityMode(src)) ;
    }


    // enabled
    // This is at the end of the function because some ODE functions modify this flag
    if( dBodyIsEnabled(src) ) {
        dBodyEnable(dst) ;
    } else {
        dBodyDisable(dst) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
