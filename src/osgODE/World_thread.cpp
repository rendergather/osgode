/*!
 * @file World_thread.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 by Rocco Martino                                   *
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
#include <osgODE/Notify>
#include <osgODE/ScopedTimer>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
void
World::threadUpdate(ooReal step_size)
{
    advanceInternal( step_size ) ;
    threadStep( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
World::threadStep(ooReal step_size)
{
    if( step_size <= 0.0 ) {
        return ;
    }


    PS_DBG3("osgODE::World::threadStep(%p, step_size=%lf)", this, step_size) ;



    m_current_step_size = step_size ;



    m_current_wind = m_wind * 0.5 * ( 1.0 + cos(2.0 * osg::PI * m_wind_frequency * m_simulation_time) ) ;



    _callObjectsCallbacks(step_size) ;



    PS_DBG3("osgODE::World::threadStep(%p, step_size=%lf): calling WorldStep", this, step_size) ;
    {
        PS_SCOPED_TIMER("WorldStep") ;

        m_world_step(m_ODE_world, step_size) ;
    }





    // !!!!!!!!!!!!
    // !!!!!!!!!!!!
    // !!!!!!!!!!!!
    // !!!!!!!!!!!!

#if 0
    // away because writes to the matrix transforms. See ThreadedManagerUodateCallback
    _callObjectsPostCallbacks(step_size) ;

    // away vbecause could add/remove nodes. See ThreadedManagerUodateCallback
    runOperationsInternal() ;
#else



    PS_DBG3("osgODE::World::threadStep(%p): calling post callbacks", this) ;



    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter = m_objects.begin() ;
    std::vector< osg::ref_ptr<ODEObject> >::iterator    iter_end = m_objects.end() ;

    while(iter != iter_end) {


        ODEObject*  o = iter->get() ;

        iter++ ;



        o->postUpdate(step_size) ;
        o->callPostUpdateCallbackInternal() ;
    }

#endif
}
/* ....................................................................... */
/* ======================================================================= */
