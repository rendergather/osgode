/*!
 * @file Events.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Events>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Events::Events(void):
    m_button_mask               ( 0 ),
    m_dirty_view_projection     ( false ),
    m_frame_counter             ( 0 ),
    m_warp_pointer_enabled      ( false ),
    m_warp_pointer_normalized   ( false ),
    m_motion                    ( false )
{
    for(unsigned int i=0; i<256; i++) {
        m_key_pressed[i] = false ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Events::Events(const Events& other, const osg::CopyOp& copyop):
    osg::Object                     ( other, copyop ),
    m_cursor_position               ( other.m_cursor_position ),
    m_normalized_cursor_position    ( other.m_normalized_cursor_position ),
    m_relative_cursor_position      ( other.m_relative_cursor_position ),
    m_button_mask                   ( other.m_button_mask ),
    m_dirty_view_projection         ( other.m_dirty_view_projection ),
    m_inverse_view_projection       ( other.m_inverse_view_projection ),
    m_frame_counter                 ( other.m_frame_counter ),
    m_view                          ( other.m_view ),
    m_view_matrix                   ( other.m_view_matrix ),
    m_warp_pointer_enabled          ( other.m_warp_pointer_enabled ),
    m_warp_pointer_normalized       ( other.m_warp_pointer_normalized ),
    m_warp_pointer                  ( other.m_warp_pointer ),
    m_motion                        ( other.m_motion )
{
    memcpy( m_key_pressed, other.m_key_pressed, 256 ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Events::~Events(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Events::copy( const Events* events )
{
    m_cursor_position =             events->m_cursor_position ;
    m_normalized_cursor_position =  events->m_normalized_cursor_position ;
    m_relative_cursor_position =    events->m_relative_cursor_position ;
    m_button_mask =                 events->m_button_mask ;
    m_dirty_view_projection =       events->m_dirty_view_projection ;
    m_inverse_view_projection =     events->m_inverse_view_projection ;
    m_frame_counter =               events->m_frame_counter ;
    m_view =                        events->m_view ;
    m_view_matrix =                 events->m_view_matrix ;
    m_warp_pointer_enabled =        events->m_warp_pointer_enabled ;
    m_warp_pointer_normalized =     events->m_warp_pointer_normalized ;
    m_warp_pointer =                events->m_warp_pointer ;
    m_motion =                      events->m_motion ;

    memcpy( m_key_pressed, events->m_key_pressed, 256 ) ;
}
/* ....................................................................... */
/* ======================================================================= */
