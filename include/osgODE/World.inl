/*!
 * @file World.inl
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

#ifndef _OSGODE_WORLD_INL
#define _OSGODE_WORLD_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setGravity(const osg::Vec3& gravity)
{
    dWorldSetGravity(m_ODE_world, gravity.x(), gravity.y(), gravity.z()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3
osgODE::World::getGravity(void) const
{
    dVector3    gravity ;
    dWorldGetGravity(m_ODE_world, gravity) ;

    return osg::Vec3(gravity[0], gravity[1], gravity[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline dWorldID
osgODE::World::getODEWorld(void)
{
    return m_ODE_world ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const dWorldID
osgODE::World::getODEWorld(void) const
{
    return m_ODE_world ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World::Objects&
osgODE::World::getObjects(void)
{
    if( ! m_objects_sorted ) {
        _sortObjectsByID() ;
    }

    return m_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World::Objects&
osgODE::World::getObjects(void) const
{
    return m_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World::Objects&
osgODE::World::getUnsortedObjects(void)
{
    return m_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World::Objects&
osgODE::World::getUnsortedObjects(void) const
{
    return m_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::sortObjectsByID(void)
{
    _sortObjectsByID() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODE::World::getCurrentFrame(void) const
{
    return m_current_frame ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setCurrentFrame(unsigned int f)
{
    m_current_frame = f ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::World::getSimulationTime(void) const
{
    return m_simulation_time ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setSimulationTime(ooReal time)
{
    m_simulation_time = time ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setWind(const osg::Vec3& wind)
{
    m_wind = wind ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::World::getWind(void) const
{
    return m_wind ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setWindFrequency(ooReal Hz)
{
    m_wind_frequency = Hz ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::World::getWindFrequency(void) const
{
    return m_wind_frequency ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::World::getCurrentWind(void) const
{
    return m_current_wind ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setAirDensity(ooReal density)
{
    m_air_density = density ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::World::getAirDensity(void) const
{
    return m_air_density ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setWorldStepFunction(WorldStepPrototype fn)
{
    m_world_step = fn ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World::WorldStepPrototype
osgODE::World::getWorldStepFunction(void) const
{
    return m_world_step ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::removeAllObjects(void)
{
    while( m_objects.size() ) {
        removeObject( *m_objects.begin() ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
template<class T>
inline void
osgODE::World::traverseObjects(T& t, void (T::*member)(ODEObject*))
{
    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;


    while( itr != itr_end ) {

        (t.*member)( *itr++ ) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::addOperation(Operation* op)
{
    m_operations.push_back(op) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::removeOperation(Operation* op)
{
    unsigned int    size = m_operations.size() ;
    unsigned int    i = 0 ;

    while( i < size ) {

        if( op != m_operations[i].get() ) {
            ++i ;
        } else {
            m_operations[i] = m_operations[--size] ;
        }
    }

    m_operations.resize(size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::removeAllOperations(void)
{
    m_operations.clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World::OperationList&
osgODE::World::getOperationList(void) const
{
    return m_operations ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline ooReal
osgODE::World::getCurrentStepSize(void) const
{
    return m_current_step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setERP(ooReal value)
{
    dWorldSetERP(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline ooReal
osgODE::World::getERP(void) const
{
    return dWorldGetERP(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setCFM(ooReal value)
{
    dWorldSetCFM(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline ooReal
osgODE::World::getCFM(void) const
{
    return dWorldGetCFM(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setQuickStepNumIterations(int value)
{
    dWorldSetQuickStepNumIterations(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline int
osgODE::World::getQuickStepNumIterations(void) const
{
    return dWorldGetQuickStepNumIterations(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setQuickStepW(ooReal value)
{
    dWorldSetQuickStepW(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline ooReal
osgODE::World::getQuickStepW(void) const
{
    return dWorldGetQuickStepW(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setContactMaxCorrectingVel(ooReal value)
{
    dWorldSetContactMaxCorrectingVel(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline ooReal
osgODE::World::getContactMaxCorrectingVel(void) const
{
    return dWorldGetContactMaxCorrectingVel(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline void
osgODE::World::setContactSurfaceLayer(ooReal value)
{
    dWorldSetContactSurfaceLayer(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline ooReal
osgODE::World::getContactSurfaceLayer(void) const
{
    return dWorldGetContactSurfaceLayer(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::advanceInternal(ooReal step_size)
{
    ++m_current_frame ;

    m_simulation_time += step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World::Objects&
osgODE::World::getActors(void)
{
    return m_actors ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World::Objects&
osgODE::World::getActors(void) const
{
    return m_actors ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setDirtyActors( bool dirty_actors )
{
    m_dirty_actors = dirty_actors ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::World::getDirtyActors(void) const
{
    return m_dirty_actors ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::swapEventsBuffer(void)
{
    if( ! m_front_events_buffer ) {
        m_front_events_buffer = new Events() ;
    }


    if( ! m_back_events_buffer ) {
        m_back_events_buffer = new Events() ;
    }


    osg::ref_ptr<Events>    tmp = m_front_events_buffer ;

    m_front_events_buffer = m_back_events_buffer ;

    m_back_events_buffer = tmp ;


    m_back_events_buffer->clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setFrontEventsBuffer( Events* front_events_buffer )
{
    m_front_events_buffer = front_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Events*
osgODE::World::getFrontEventsBuffer(void)
{
    return m_front_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Events*
osgODE::World::getFrontEventsBuffer(void) const
{
    return m_front_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setBackEventsBuffer( Events* back_events_buffer )
{
    m_back_events_buffer = back_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Events*
osgODE::World::getBackEventsBuffer(void)
{
    return m_back_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Events*
osgODE::World::getBackEventsBuffer(void) const
{
    return m_back_events_buffer ;
}
/* ....................................................................... */
/* ======================================================================= */





#endif /* _OSGODE_WORLD_INL */
