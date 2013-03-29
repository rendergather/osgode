/*!
 * @file Manager.cpp
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
#include <osgODE/Manager>
#include <osgODE/StaticWorld>
#include <osgODE/Notify>
#include <osgODE/ScopedTimer>
#include <osgODE/ManagerUpdateCallback>
#include <osgODE/InterpolableMatrixTransform>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#ifndef MANAGER_DEFAULT_STEP_SIZE
# define MANAGER_DEFAULT_STEP_SIZE (1.0/60.0)
#endif
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
namespace {
    // lol .. cannot resist
    class PusherVisitor: public osg::NodeVisitor
    {
    public:
        PusherVisitor(void): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}

        virtual void    apply(osg::MatrixTransform& transform)
        {
            InterpolableMatrixTransform*    xform = dynamic_cast<InterpolableMatrixTransform*>( & transform ) ;


            if( xform ) {
                xform->push() ;
            }


            traverse(transform) ;
        }

    } ;



    class InterpolatorVisitor: public osg::NodeVisitor
    {
    public:
        InterpolatorVisitor(void): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), m_t(1.0) {}

        virtual void    apply(osg::MatrixTransform& transform)
        {
            InterpolableMatrixTransform*    xform = dynamic_cast<InterpolableMatrixTransform*>( & transform ) ;


            if( xform ) {
                xform->interpolate(m_t) ;
            }


            traverse(transform) ;
        }

        void    setT(double t)
        {   m_t = t ;   }

    private:
        double  m_t ;

    } ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Manager::Manager(void):
    m_delta(0.0),
    m_done(false),
    m_step_size(MANAGER_DEFAULT_STEP_SIZE),
    m_time_multiplier(1.0),
    m_auto_start_thread(false),
    m_accept_visitors(false),
    m_interpolation_enabled(false),
    m_force_update_traversal(false),
    m_push_nv( new PusherVisitor() ),
    m_interpolate_nv( new InterpolatorVisitor() )
{

    {
        // Call the static world constructor to inizialize the library
        osg::ref_ptr<osg::Object>   o = StaticWorld::instance() ;
    }


    m_timer.setStartTick() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Manager::Manager(const Manager& other, const osg::CopyOp& copyop):
    osg::Node(other, copyop),

    m_delta(0.0),
    m_done(false),
    m_step_size(other.m_step_size),
    m_time_multiplier(other.m_time_multiplier),
    m_auto_start_thread(other.m_auto_start_thread),
    m_accept_visitors(other.m_accept_visitors),
    m_interpolation_enabled( other.m_interpolation_enabled ),
    m_push_nv( other.m_push_nv ),
    m_interpolate_nv( other.m_interpolate_nv )

{
    setForceUpdateTraversal( getForceUpdateTraversal() ) ;

    m_world = static_cast<World*>( other.m_world->clone(copyop) ) ;

    m_timer.setStartTick() ;
}
/* ....................................................................... */
/* ======================================================================= */





/* ======================================================================= */
/* ....................................................................... */
Manager::~Manager(void)
{
    PS_DBG3("osgODE::Manager::~Manager(%p)", this) ;


    setDone(true) ;

    while( isRunning() ) {
#ifdef WIN32
        Sleep(1) ;
#else
        usleep(1) ;
#endif
    }
}
/* ....................................................................... */
/* ======================================================================= */





/* ======================================================================= */
/* ....................................................................... */
void
Manager::setup(bool separate_thread, bool accept_visitors, double step_size)
{
    PS_DBG( "osgODE::Manager::setup(%p, separate_thread=%d, accept_visitors=%d, step_size=%lf)",
            this, separate_thread, accept_visitors, step_size) ;

    setAutoStartThread( separate_thread ) ;

    if( ! separate_thread ) {
        addUpdateCallback( new osgODE::ManagerUpdateCallback() ) ;
    }

    setAcceptVisitors( accept_visitors ) ;

    setStepSize( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::frame(double step_size)
{
    if( step_size <= 0.0 ) {
        return ;
    }



    PS_DBG3("osgODE::Manager::frame(%p, step_size=%lf)", this, step_size) ;

    PS_SCOPED_TIMER("Manager::frame") ;


    m_delta += step_size ;

    const double    step_done = m_delta > m_step_size ;


    if(m_world.valid()) {


        m_world->writeLock() ;


        while( m_delta > m_step_size ) {

            m_delta -= m_step_size ;

            m_world->callUpdateCallbackInternal() ;
            m_world->update(step_size) ;
            m_world->callPostUpdateCallbackInternal() ;
        }


        if( step_done && m_interpolation_enabled ) {
            _pushTransforms() ;
        }



        m_world->writeUnlock() ;


        dirtyBound() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::logicFrame(double step_size)
{
    if(     ( step_size <= 0.0 )    ||    ( ! m_interpolation_enabled )    ) {
        return ;
    }


    const double    t = osg::clampTo( m_delta + step_size / m_step_size, 0.0, 1.0 ) ;


//     PS_DBG3("osgODE::Manager::logicFrame(%p, step_size=%lf): t = %lf", this, step_size, t) ;

    _interpolate( t ) ;
    (void)t ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::run(void)
{
    PS_DBG("osgODE::Manager::run(%p)", this) ;




    PS_DBG("osgODE::Manager::run(%p): dAllocateODEDataForThread", this) ;
    dAllocateODEDataForThread(dAllocateMaskAll) ;


    setDone(false) ;

    m_timer.setStartTick() ;


    if(m_world.valid()) {
        m_world->setCurrentFrame(0) ;
    }



    while( ! m_done ) {

        const double    t = m_timer.time_s() ;

        if( t >= m_step_size ) {

            this->frame( t ) ;

            m_timer.setStartTick() ;

            logicFrame( m_delta ) ;

        } else {

            logicFrame( t ) ;

        }


        OpenThreads::Thread::YieldCurrentThread() ;
//         OpenThreads::Thread::microSleep( (m_step_size - m_delta) * 1.0e6 * 1.0/1.0 ) ;

    } // while ! done




    PS_DBG("osgODE::Manager::run(%p) returns", this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::traverse(osg::NodeVisitor& nv)
{
    if( m_auto_start_thread && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {

        m_auto_start_thread = false ;


        if( ! isRunning() ) {

            PS_DBG("osgODE::Manager::traverse(%p, nv=%p): this->startThread", this, &nv) ;
            this->startThread() ;
        }
    }




    // divert the visitor
    if( m_accept_visitors ) {

        PS_ASSERT1( m_world.valid() ) ;


        m_world->traverseLock() ;

        m_world->accept(nv) ;

        m_world->traverseUnlock() ;

    }


    // This does nothing
    this->osg::Node::traverse(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::BoundingSphere
Manager::computeBound(void) const
{
    osg::BoundingSphere bound ;


    if( m_world.valid() ) {


        /* NOTE
         * This lock could kill one of the thread by starvation
         */

        // !!!!!
        m_world->traverseLock() ;



        const osgODE::World::Objects&   objects = m_world->getObjects() ;

        osgODE::World::Objects::const_iterator  itr = objects.begin() ;
        osgODE::World::Objects::const_iterator  itr_end = objects.end() ;

        while( itr != itr_end ) {
            bound.expandBy( (*itr++)->getBound() ) ;
        }



        // !!!!!
        m_world->traverseUnlock() ;


    } // if( m_world.valid() )



    return bound ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::_pushTransforms(void)
{
    m_world->accept( *m_push_nv ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::_interpolate(double t)
{
    static_cast<InterpolatorVisitor*>( m_interpolate_nv.get() )->setT(t) ;

    m_world->accept( *m_interpolate_nv ) ;
}
/* ....................................................................... */
/* ======================================================================= */
