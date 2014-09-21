/*!
 * @file Manager.cpp
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
#include <osgODE/Manager>
#include <osgODE/StaticWorld>
#include <osgODE/Notify>
#include <osgODE/ScopedTimer>
#include <osgODE/ManagerUpdateCallback>

#include <osg/Timer>

#ifndef WIN32
    #include <unistd.h>
#endif
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
    m_delta                     ( 0.0 ),
    m_step_size                 ( MANAGER_DEFAULT_STEP_SIZE ),
    m_time_multiplier           ( 1.0 ),
    m_accept_visitors           ( false ),
    m_force_update_traversal    ( false )
{

    // Call the static world constructor to inizialize the library
    osg::ref_ptr<osg::Object>   o = StaticWorld::instance() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Manager::Manager(const Manager& other, const osg::CopyOp& copyop):
    osg::Node(other, copyop),

    m_delta             ( 0.0 ),
    m_step_size         ( other.m_step_size ),
    m_time_multiplier   ( other.m_time_multiplier ),
    m_accept_visitors   ( other.m_accept_visitors )

{
    setForceUpdateTraversal( getForceUpdateTraversal() ) ;

    m_world = static_cast<World*>( other.m_world->clone(copyop) ) ;
}
/* ....................................................................... */
/* ======================================================================= */





/* ======================================================================= */
/* ....................................................................... */
Manager::~Manager(void)
{
    PS_DBG3("osgODE::Manager::~Manager(%p)", this) ;
}
/* ....................................................................... */
/* ======================================================================= */





/* ======================================================================= */
/* ....................................................................... */
void
Manager::setup(bool accept_visitors, double step_size)
{
    PS_DBG( "osgODE::Manager::setup(%p, accept_visitors=%d, step_size=%lf)",
            this, accept_visitors, step_size) ;

    setUpdateCallback( new osgODE::ManagerUpdateCallback() ) ;

    setAcceptVisitors( accept_visitors ) ;

    setStepSize( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Manager::frame(double dt)
{
    bool    advanced = false ;

    if( dt <= 0.0 ) {
        return advanced ;
    }



    PS_DBG3("osgODE::Manager::frame(%p, dt=%lf)", this, dt) ;

    PS_SCOPED_TIMER("Manager::frame") ;


    m_delta += dt ;


    if(m_world.valid()) {

        advanced =   m_delta >= m_step_size ;



        while( m_delta >= m_step_size ) {

            m_delta -= m_step_size ;


            m_world->callUpdateCallbackInternal() ;

            m_world->update(m_step_size) ;

            m_world->callPostUpdateCallbackInternal() ;
        }


        dirtyBound() ;
    }


    return advanced ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Manager::traverse(osg::NodeVisitor& nv)
{
    // divert the visitor
    if( m_accept_visitors && m_world.valid() ) {

        m_world->accept(nv) ;

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



        const osgODE::World::Objects&   objects = m_world->getUnsortedObjects() ;

        osgODE::World::Objects::const_iterator  itr = objects.begin() ;
        osgODE::World::Objects::const_iterator  itr_end = objects.end() ;

        while( itr != itr_end ) {
            bound.expandBy( (*itr++)->getBound() ) ;
        }


    } // if( m_world.valid() )



    return bound ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManager*
Manager::asThreadedManager(void)
{
    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */
