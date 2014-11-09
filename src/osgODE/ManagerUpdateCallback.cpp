/*!
 * @file ManagerUpdateCallback.cpp
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
#include <osgODE/ManagerUpdateCallback>
#include <osgODE/Manager>
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
ManagerUpdateCallback::ManagerUpdateCallback(void):
    m_last_s                ( -1.0 ),
    m_delta                 ( 0.0 ),
    m_max_step_size         ( -1.0 ),
    m_max_frames_per_update ( 0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerUpdateCallback::ManagerUpdateCallback(const ManagerUpdateCallback& other, const osg::CopyOp& copyop):
    osg::NodeCallback       ( other, copyop ),
    m_last_s                ( other.m_last_s ),
    m_delta                 ( other.m_delta ),
    m_max_step_size         ( other.m_max_step_size ),
    m_max_frames_per_update ( other.m_max_frames_per_update )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerUpdateCallback::~ManagerUpdateCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ManagerUpdateCallback::operator()(osg::Node* n, osg::NodeVisitor* nv)
{
    PS_ASSERT1( dynamic_cast<Manager*> (n) ) ;


    Manager*        manager = static_cast<Manager*>(n) ;

    const ooReal    step_size = manager->getStepSize() ;


    const ooReal    sim_time = nv->getFrameStamp()->getSimulationTime() ;


    if( m_last_s > 0.0  &&  sim_time > m_last_s ) {

        m_delta += (sim_time - m_last_s) * manager->getTimeMultiplier() ;

        m_last_s = sim_time ;


        if( m_max_step_size > 0.0 ) {
            m_delta = osg::minimum( m_delta, m_max_step_size ) ;
        }




        if( m_delta >= step_size ) {

            World*  world = manager->getWorld() ;
            PS_ASSERT1( world ) ;

            world->swapEventsBuffer() ;

        }




        if( m_max_frames_per_update == 0 ) {

            while( m_delta >= step_size ) {
                manager->frame( step_size ) ;
                m_delta -= step_size ;
            }

        } else {
            unsigned int    frame_count = 0 ;

            while( m_delta >= step_size && frame_count++ < m_max_frames_per_update ) {
                manager->frame(step_size) ;
                m_delta -= step_size ;
            }


            while( m_delta >= step_size ) {
                m_delta -= step_size ;
            }
        }



        const Events::ViewMatrixRequests&   view_matrix_requests = manager->getWorld()->getFrontEventsBuffer()->getViewMatrixRequests() ;

        for( unsigned int i=0; i<view_matrix_requests.size(); i++ ) {
            view_matrix_requests[i].first->getCamera()->setViewMatrix( view_matrix_requests[i].second ) ;
        }



    } else {

        m_last_s = sim_time ;

    }


    traverse(n, nv) ;
}
/* ....................................................................... */
/* ======================================================================= */
