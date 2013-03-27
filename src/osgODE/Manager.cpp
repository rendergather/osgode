/*!
 * @file Manager.cpp
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
#include <osgODE/Manager>
#include <osgODE/StaticWorld>
#include <osgODE/Notify>
#include <osgODE/ScopedTimer>
#include <osgODE/ManagerUpdateCallback>
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
Manager::Manager(void):
    m_delta(0.0),
    m_done(false),
    m_step_size(MANAGER_DEFAULT_STEP_SIZE),
    m_time_multiplier(1.0),
    m_auto_start_thread(false),
    m_accept_visitors(false),
    m_use_non_constant_step_size(false),
    m_force_update_traversal(false)
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
    m_use_non_constant_step_size(other.m_use_non_constant_step_size)

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
    PS_DBG3("osgODE::Manager::frame(%p, step_size=%lf)", this, step_size) ;

    PS_SCOPED_TIMER("Manager::frame") ;



    if( step_size <= 0.0 ) {
        step_size = m_step_size ;
    }


    if(m_world.valid()) {


        m_world->writeLock() ;


        m_world->callUpdateCallbackInternal() ;
        m_world->update(step_size) ;
        m_world->callPostUpdateCallbackInternal() ;



        m_world->writeUnlock() ;


        dirtyBound() ;
    }
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



/*=======================*/
/* these are taken into account only when
   m_use_non_constant_step_size == true */

    // the number of frames on which we calculate the mean
    const unsigned int  N = 100 ;

    // array fill
    unsigned int        F = 0 ;

    // calculated mean over last frames
    double  M = 0.0 ;

    // current update spent time
    double  elapsed = 0.0 ;


    // helper
    osg::Timer  aux_timer ;
/*=======================*/


    setDone(false) ;


    if(m_world.valid()) {
        m_world->setCurrentFrame(0) ;
    }



    resetTimer() ;



    while( ! m_done ) {

        // accumulated time
        m_delta += m_timer.time_s() ;
        m_timer.setStartTick() ;

        // this maximum step size avoids degenerations
        if(m_delta > 0.1)   m_delta = 0.1 ;


        // wait until step_size is reached
        while( m_delta < m_step_size ) {

            OpenThreads::Thread::microSleep( (m_step_size - m_delta) * 1.0e6 ) ;
//             OpenThreads::Thread::YieldCurrentThread() ;

            m_delta += m_timer.time_s() * m_time_multiplier ;

            m_timer.setStartTick() ;
        }



        if(  ! m_use_non_constant_step_size ) {

            //
            // constant step size
            //

            while( m_delta >= m_step_size ) {

                frame(m_step_size) ;

                m_delta -= m_step_size ;
            }



        } else {



// *****************************************************************************
// USE NON CONSTANT STEP SIZE == TRUE                                          *
// Tengo in considerazione quanto tempo impiega la frame a tornare e mi baso   *
// su quel valore per modificare lo step_size.                                 *
// Ancora non so bene come fare                                                *
// *****************************************************************************

            while( m_delta >= m_step_size ) {

                //
                // update e tempo impiegato dall'update
                //

                aux_timer.setStartTick() ;

                // the frame must take into account the step size and the time
                // it takes to do its job, so the actual step size is the sum
                // of this two times
                frame(m_step_size + M) ;

                elapsed = aux_timer.time_s() ;

                F = osg::minimum(N, F + 1) ;

                M = ( (M * (F-1)) + elapsed ) / F ;




                // add the exceeding time
                m_delta -= m_step_size + M ;


            }

        } // using m_use_non_constant_step_size

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
