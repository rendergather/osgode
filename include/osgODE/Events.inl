/*!
 * @file Events.inl
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

#ifndef _OSGODE_EVENTS_INL
#define _OSGODE_EVENTS_INL




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Events::setEventList( const EventList& event_list )
{
    m_event_list = event_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Events::EventList&
osgODE::Events::getEventList(void)
{
    return m_event_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Events::EventList&
osgODE::Events::getEventList(void) const
{
    return m_event_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Events::enqueue( const osgGA::GUIEventAdapter* ea, osg::View* aa )
{
    m_event_list.push_back( Event(ea, aa) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Events::clear(void)
{
    m_event_list.clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Events::requestViewMatrix(osg::View* view, const osg::Matrix& matrix)
{
    m_view_matrix_requests.push_back( std::pair<osg::View*, osg::Matrix>(view, matrix) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Events::clearViewMatrixRequests(void)
{
    m_view_matrix_requests.clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Events::ViewMatrixRequests&
osgODE::Events::getViewMatrixRequests(void) const
{
    return m_view_matrix_requests ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_EVENTS_INL */
