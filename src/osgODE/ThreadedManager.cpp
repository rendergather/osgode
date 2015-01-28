/*!
 * @file ThreadedManager.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 - 2015 by Rocco Martino ...                        *
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
#include <osgODE/ThreadedManager>
#include <osgODE/ScopedTimer>
#include <osgODE/Notify>

#include <osg/Timer>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
ThreadedManager::ThreadedManager(void):
    m_time          ( -1.0 ),
    m_done          ( false ),
    m_paused        ( false ),
    m_pause_request ( false ),
    m_rdy           ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManager::ThreadedManager(const ThreadedManager& other, const osg::CopyOp& copyop):
    Manager         ( other, copyop ),
    m_time          ( -1.0 ),
    m_done          ( false ),
    m_paused        ( false ),
    m_pause_request ( false ),
    m_rdy           ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManager::~ThreadedManager(void)
{
    done( true ) ;

    while( isRunning() ) {
        OpenThreads::Thread::YieldCurrentThread() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ThreadedManager::run(void)
{
    dInitODE2(0) ;
    dAllocateODEDataForThread( (unsigned int) dAllocateMaskAll ) ;


    while( ! m_done )  {


        if( m_pause_request) {
            m_paused = true ;
        }



        while( m_paused ) {

            if( m_done ) {
                goto DONE ;
            }

            OpenThreads::Thread::YieldCurrentThread() ;
        }



        ooReal  t_s = osg::Timer::instance()->time_s() ;

        if( m_time < 0.0 ) {
            m_time = t_s ;
        }

        ooReal  dt = t_s - m_time ;


        dt = osg::minimum( dt, (ooReal)1.0e-1 ) ;


        if( dt >= getStepSize() ) {
            frame( dt ) ;
            m_time = t_s ;
            m_rdy = true ;
        } else {
            OpenThreads::Thread::YieldCurrentThread() ;
        }

    }




DONE:
    return ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
ThreadedManager::frame(ooReal dt)
{
    bool    advanced = false ;

    if( dt <= 0.0 ) {
        return advanced ;
    }



    PS_DBG3("oo::ThreadedManager::frame(%p, dt=%lf)", this, dt) ;

    PS_SCOPED_TIMER("ThreadedManager::frame") ;


    m_delta += dt ;

    World*          world = getWorld() ;

    PS_ASSERT1( world ) ;

    if( world ) {


        // moved to ThreadedManagerUpdateCallback
//         world->swapEventsBuffer() ;

        const ooReal    step_size= getStepSize() ;

        advanced =   m_delta >= step_size ;



        while( m_delta >= step_size ) {

            m_delta -= step_size ;


            world->callUpdateCallbackInternal() ;

            world->threadUpdate(step_size) ;

            world->callPostUpdateCallbackInternal() ;
        }


        dirtyBound() ;
    }


    return advanced ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManager*
ThreadedManager::asThreadedManager(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
