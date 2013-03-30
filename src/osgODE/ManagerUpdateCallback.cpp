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
    m_last_tick(0),
    m_delta(0.0),
    m_max_frames_per_update(0)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerUpdateCallback::ManagerUpdateCallback(const ManagerUpdateCallback& other, const osg::CopyOp& copyop):
    osg::NodeCallback(other, copyop),
    m_last_tick(other.m_last_tick),
    m_delta(other.m_delta),
    m_max_frames_per_update( other.m_max_frames_per_update )
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
    osg::Timer* timer = osg::Timer::instance() ;

    Manager*        manager = static_cast<Manager*>(n) ;

    const double    step_size = manager->getStepSize() ;



    if( m_last_tick != 0 ) {

        osg::Timer_t    cur_tick = timer->tick() ;

        m_delta += timer->delta_s(m_last_tick, cur_tick) * manager->getTimeMultiplier() ;

        m_last_tick = cur_tick ;


        if( m_max_frames_per_update == 0 ) {

            while( m_delta >= step_size ) {
                manager->frame(step_size) ;
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

    } else {

        m_last_tick = timer->tick() ;

    }


    traverse(n, nv) ;
}
/* ....................................................................... */
/* ======================================================================= */
