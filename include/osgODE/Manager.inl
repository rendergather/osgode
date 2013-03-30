/*!
 * @file Manager.inl
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

#ifndef _OSGODE_MANAGER_INL
#define _OSGODE_MANAGER_INL

/* ======================================================================= */
/* ....................................................................... */
#include <osg/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setDone(bool done)
{
    m_done = done ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::Manager::getDone(void) const
{
    return m_done ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setStepSize(double step_size)
{
    m_step_size = step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::Manager::getStepSize(void) const
{
    return m_step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setTimeMultiplier(double multiplier)
{
    m_time_multiplier = multiplier ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::Manager::getTimeMultiplier(void) const
{
    return m_time_multiplier ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setWorld(osgODE::World* world)
{
    m_world = world ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World*
osgODE::Manager::getWorld(void)
{
    return m_world.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World*
osgODE::Manager::getWorld(void) const
{
    return m_world.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setAutoStartThread(bool flag)
{
    m_auto_start_thread = flag ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::Manager::getAutoStartThread(void) const
{
    return m_auto_start_thread ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setAcceptVisitors(bool flag)
{
    m_accept_visitors = flag ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::Manager::getAcceptVisitors(void) const
{
    return m_accept_visitors ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Manager::setForceUpdateTraversal(bool flag)
{
    if( flag == m_force_update_traversal ) {
        return ;
    }

    m_force_update_traversal = flag ;

    if( flag ) {
        setNumChildrenRequiringUpdateTraversal( getNumChildrenRequiringUpdateTraversal() + 1 ) ;
    } else {
        setNumChildrenRequiringUpdateTraversal( getNumChildrenRequiringUpdateTraversal() - 1 ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::Manager::getForceUpdateTraversal(void) const
{
    return m_force_update_traversal ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_MANAGER_INL */
