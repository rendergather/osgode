/*!
 * @file World.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2014 by Rocco Martino                            *
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
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/Container>
#include <osgODE/Notify>
#include <osgODE/ScopedTimer>

#include <algorithm>

#if ! ( defined(ANDROID)  ||  defined(WIN32) )
    #include <regex.h>
#endif
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
World::World(void):
    m_world_step            ( dWorldStep ),
    m_current_frame         ( 0 ),
    m_simulation_time       ( 0.0 ),
    m_objects_sorted        ( true ),
    m_wind_frequency        ( 0.0 ),
    m_air_density           ( 1.2929 ),
    m_current_step_size     ( 0.0 ),
    m_dirty_actors          ( false )
{


    m_ODE_world = dWorldCreate() ;

    dWorldSetGravity                        ( m_ODE_world, 0.0, 0.0, -9.80665 ) ;
    dWorldSetMaxAngularSpeed                ( m_ODE_world, FLT_MAX ) ;
    dWorldSetLinearDamping                  ( m_ODE_world, 0.01 ) ;
    dWorldSetAngularDamping                 ( m_ODE_world, 0.01 ) ;
    dWorldSetLinearDampingThreshold         ( m_ODE_world, 0.01 ) ;
    dWorldSetAngularDampingThreshold        ( m_ODE_world, 0.01 ) ;
    dWorldSetAutoDisableLinearThreshold     ( m_ODE_world, 0.1 ) ;
    dWorldSetAutoDisableAngularThreshold    ( m_ODE_world, 0.1 ) ;
    dWorldSetAutoDisableFlag                ( m_ODE_world, 0 ) ;
    dWorldSetAutoDisableSteps               ( m_ODE_world, 180 ) ;
    dWorldSetAutoDisableTime                ( m_ODE_world, 3.0 ) ;
    dWorldSetAutoDisableAverageSamplesCount ( m_ODE_world, 6 ) ;



    dWorldSetERP(m_ODE_world, 0.2) ;
    dWorldSetCFM(m_ODE_world, 1.0e-5) ;

    setContactMaxCorrectingVel(FLT_MAX) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
World::World(const World& other, const osg::CopyOp& copyop):
    osgODE::ODEObject       ( other, copyop ),
    m_world_step            ( other.m_world_step ),
    m_current_frame         ( other.m_current_frame ),
    m_simulation_time       ( other.m_simulation_time ),
    m_objects_sorted        ( true ),
    m_wind                  ( other.m_wind ),
    m_wind_frequency        ( other.m_wind_frequency ),
    m_air_density           ( other.m_air_density ),
    m_current_wind          ( other.m_current_wind ),
    m_current_step_size     ( other.m_current_step_size ),
    m_events                ( osg::clone( other.m_events.get(), copyop ) ),
    m_dirty_actors          ( other.m_dirty_actors ),
    m_actors                ( other.m_actors )
{


    m_ODE_world = dWorldCreate() ;


    _cloneODEWorld(other.m_ODE_world, m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
World::~World(void)
{
    removeAllObjects() ;

    dWorldDestroy(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::addObject(ODEObject* obj)
{
    PS_DBG2("osgODE::World::addObject(%p, obj=%p)", this, obj) ;

    PS_ASSERT1(obj != NULL) ;



    bool    ret_val = obj->addToWorldInternal(this) ;


    if(ret_val) {


        obj->setWorldInternal(this) ;

        m_objects.push_back(obj) ;

        m_objects_sorted = false ;


    } else {
        obj->setWorldInternal(NULL) ;
    }


#if IS_X86_64
    PS_DBG2("osgODE::World(%p): Objects: %lu", this, m_objects.size()) ;
#else
    PS_DBG2("osgODE::World(%p): Objects: %u", this, m_objects.size()) ;
#endif

    return ret_val ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::removeObject(ODEObject* obj)
{
    PS_DBG2("osgODE::World::removeObject(%p, obj=%p)", this, obj) ;

    PS_ASSERT1(obj != NULL) ;



    bool    ret_val = obj->removeFromWorldInternal(this) ;


    if(ret_val) {


        obj->setWorldInternal(NULL) ;

        // The objects can be messy, so I need to do a linear search

        unsigned int  size = m_objects.size() ;

        unsigned int    i = 0 ;

        while(i < size) {

            osgODE::ODEObject* current = m_objects[i].get() ;


            if(current == obj) {


                m_objects[i] = m_objects[ --size ] ;

                m_objects_sorted = false ;

                continue ;


            } else {

                i++ ;

            }
        }


        m_objects.resize(size) ;
    }


#if IS_X86_64
    PS_DBG2("osgODE::World(%p): Objects: %lu", this, m_objects.size()) ;
#else
    PS_DBG2("osgODE::World(%p): Objects: %u", this, m_objects.size()) ;
#endif

    return ret_val ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::update(double step_size)
{
    advanceInternal( step_size ) ;
    step( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::step(double step_size)
{
    if( step_size <= 0.0 ) {
        return ;
    }


    PS_DBG3("osgODE::World::step(%p, step_size=%lf)", this, step_size) ;



    m_current_step_size = step_size ;



    m_current_wind = m_wind * 0.5 * ( 1.0 + cos(2.0 * osg::PI * m_wind_frequency * m_simulation_time) ) ;



    _callObjectsCallbacks(step_size) ;



    PS_DBG3("osgODE::World::step(%p, step_size=%lf): calling WorldStep", this, step_size) ;
    {
        PS_SCOPED_TIMER("WorldStep") ;

        m_world_step(m_ODE_world, step_size) ;
    }



    _callObjectsPostCallbacks(step_size) ;



    runOperationsInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::_callObjectsCallbacks(double step_size)
{
    PS_DBG3("osgODE::World::_callObjectsCallbacks(%p, step_size=%f)", this, step_size) ;


    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter = m_objects.begin() ;
    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter_end = m_objects.end() ;

    while(iter != iter_end) {


        ODEObject*  o = iter->get() ;

        iter++ ;


        o->update(step_size) ;
        o->callUpdateCallbackInternal() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::_callObjectsPostCallbacks(double step_size)
{
    PS_DBG3("osgODE::World::_callObjectsPostCallbacks(%p, step_size=%f)", this, step_size) ;



    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter = m_objects.begin() ;
    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter_end = m_objects.end() ;

    while(iter != iter_end) {


        ODEObject*  o = iter->get() ;

        iter++ ;



        RigidBody*  body = o->asRigidBody() ;

        if( body ) {
            body->updateTransformInternal() ;
        }



        o->postUpdate(step_size) ;
        o->callPostUpdateCallbackInternal() ;
    }





    if( m_events.valid() ) {

        Events::ViewMatrix& vm = m_events->getViewMatrix() ;

        if( vm.getPriority() > 0 ) {
            m_events->getView()->getCamera()->setViewMatrix( vm.getMatrix() ) ;
            vm.setPriority( 0 ) ;
        }

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::runOperationsInternal(void)
{
    if( m_operations.size() ) {

        OperationList   operations = m_operations ;
        m_operations.clear() ;

        PS_DBG3("osgODE::World::runOperationsInternal(%p)", this) ;

        OperationList::iterator itr = operations.begin() ;
        OperationList::iterator itr_end = operations.end() ;


        while( itr != itr_end ) {

            Operation*  op = *itr++ ;

            PS_ASSERT1( op != NULL ) ;


            PS_DBG3("osgODE::World::runOperationsInternal(%p): new operation %p", this, op) ;

            (*op)(this) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    struct comp_class {
        bool    operator()(ODEObject* a, ODEObject* b) {
            return (a->getID() < b->getID() ) ;
        }
    } comp_obj ;
}

void
World::_sortObjectsByID(void)
{
    std::sort(m_objects.begin(), m_objects.end(), comp_obj) ;
    m_objects_sorted = true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ODEObject*
World::getObjectByID(unsigned int ID)
{
    if( ! m_objects_sorted ) {
        _sortObjectsByID() ;
    }


    //
    // The objects are ID ordered, so I can do a binary search
    //


    ODEObject*  found = NULL ;

    int first = 0 ;
    int last = m_objects.size() - 1 ;

    while(first <= last) {

        int mid = (first + last) / 2 ;


        ODEObject*  obj = m_objects[mid].get() ;



        if( ID > obj->getID() ) {

            first = mid + 1 ;


        } else if( ID < obj->getID() ) {

            last = mid - 1 ;


        } else {

            found = obj ;
            break ;
        }
    }

   return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ODEObject*
World::getObjectByName(const std::string& name, bool traverse_containers)
{
    //
    // linear search
    //


    ODEObject*  found = NULL ;

    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;


    if( ! traverse_containers ) {

        while( itr != itr_end ) {
            ODEObject*  obj = *itr++ ;

            if( obj->getName() == name ) {
                found = obj ;
                break ;
            }
        }

    } else {

        while( itr != itr_end ) {
            ODEObject*  obj = *itr++ ;

            if( obj->getName() == name ) {
                found = obj ;
                break ;
            }


            osgODE::Container*  container = obj->asContainer() ;


            if( container ) {
                found = container->getObjectByName( name ) ;

                if( found ) {
                    break ;
                }
            }
        }
    }

    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::getObjectsByRegexName(const std::string& pattern, Objects& result, int regcomp_cflags)
{
    bool    found = false ;


#if defined(ANDROID)  ||  defined(WIN32)

    (void) pattern ;
    (void) result ;
    (void) regcomp_cflags ;

#ifdef ANDROID
    PS_FATAL("osgODE::World::getObjectsByRegexName(%p): not supported in Android build", this) ;
#else
    PS_FATAL("osgODE::World::getObjectsByRegexName(%p): not supported in Windows build", this) ;
#endif



    PS_ASSERT1( false ) ;



#else /* defined(ANDROID)  ||  defined(WIN32) */


    //
    // linear search
    //

    regex_t     regex ;
    regmatch_t  pmatch ;

    regcomp( &regex, pattern.c_str(), regcomp_cflags ) ;

    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;

    while( itr != itr_end ) {
        ODEObject*  obj = *itr++ ;

        if( regexec( &regex, obj->getName().c_str(), 1, &pmatch, 0 ) == 0 ) {
            result.push_back( obj ) ;
            found = true ;
        }
    }


    regfree( &regex ) ;

#endif /* defined(ANDROID)  ||  defined(WIN32) */

    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::copyWorld(const World* other)
{
    m_wind = other->m_wind ;
    m_air_density = other->m_air_density ;

    _cloneODEWorld(other->m_ODE_world, m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::_cloneODEWorld(dWorldID src, dWorldID dst)
{
    PS_DBG2("osgODE::World::_cloneODEWorld(src=%p, dst=%p)", src, dst) ;

    // general functions
    {
        dVector3    gravity ;
        dWorldGetGravity( src, gravity ) ;
        dWorldSetGravity( dst, gravity[0], gravity[1], gravity[2] ) ;

        dWorldSetERP(                               dst, dWorldGetERP(                              src)) ;
        dWorldSetCFM(                               dst, dWorldGetCFM(                              src)) ;
        dWorldSetAutoDisableFlag(                   dst, dWorldGetAutoDisableFlag(                  src)) ;
        dWorldSetAutoDisableLinearThreshold(        dst, dWorldGetAutoDisableLinearThreshold(       src)) ;
        dWorldSetAutoDisableAngularThreshold(       dst, dWorldGetAutoDisableAngularThreshold(      src)) ;
        dWorldSetAutoDisableSteps(                  dst, dWorldGetAutoDisableSteps(                 src)) ;
        dWorldSetAutoDisableTime(                   dst, dWorldGetAutoDisableTime(                  src)) ;
        dWorldSetAutoDisableAverageSamplesCount(    dst, dWorldGetAutoDisableAverageSamplesCount(   src)) ;
    }


    // stepping functions
    {
        dWorldSetQuickStepNumIterations(        dst, dWorldGetQuickStepNumIterations(       src)) ;
        dWorldSetQuickStepW(                    dst, dWorldGetQuickStepW(                   src)) ;
    }


    // damping
    {
        dWorldSetLinearDamping(                 dst, dWorldGetLinearDamping(                src)) ;
        dWorldSetAngularDamping(                dst, dWorldGetAngularDamping(               src)) ;
        dWorldSetLinearDampingThreshold(        dst, dWorldGetLinearDampingThreshold(       src)) ;
        dWorldSetAngularDampingThreshold(       dst, dWorldGetAngularDampingThreshold(      src)) ;
        dWorldSetMaxAngularSpeed(               dst, dWorldGetMaxAngularSpeed(              src)) ;
    }


    // contact parameters
    {
        dWorldSetContactMaxCorrectingVel(       dst, dWorldGetContactMaxCorrectingVel(      src)) ;
        dWorldSetContactSurfaceLayer(           dst, dWorldGetContactSurfaceLayer(          src)) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::addToWorldInternal(World* world)
{
    PS_DBG2("osgODE::World::addToWorldInternal(%p, world=%p)", this, world) ;

    return ODEObject::addToWorldInternal( world ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::removeFromWorldInternal(World* world)
{
    PS_DBG2("osgODE::World::removeFromWorldInternal(%p, world=%p)", this, world) ;

    return ODEObject::removeFromWorldInternal( world ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
World*
World::asWorld(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::accept(osg::NodeVisitor& nv)
{
    traverse(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::traverse(osg::NodeVisitor& nv)
{
    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;


    while( itr != itr_end ) {

        ODEObject*  obj = *itr++ ;

        PS_ASSERT1( obj ) ;

        obj->accept(nv) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::updateTransformsInternal(void)
{
    PS_DBG3("osgODE::World::updateRigidBodyTransformsInternal(%p)", this) ;



    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter = m_objects.begin() ;
    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter_end = m_objects.end() ;

    while(iter != iter_end) {

        (*iter++)->updateTransformInternal() ;
    }





    if( m_events.valid() ) {

        Events::ViewMatrix& vm = m_events->getViewMatrix() ;

        if( vm.getPriority() > 0 ) {
            m_events->getView()->getCamera()->setViewMatrix( vm.getMatrix() ) ;
            vm.setPriority( 0 ) ;
        }

    }
}
/* ....................................................................... */
/* ======================================================================= */
