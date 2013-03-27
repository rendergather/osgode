/*!
 * @file World.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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
osgODE::World::readLock(void)
{
    m_read_write_mutex.readLock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::readUnlock(void)
{
    m_read_write_mutex.readUnlock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::writeLock(void)
{
    m_read_write_mutex.writeLock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::writeUnlock(void)
{
    m_read_write_mutex.writeUnlock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::traverseLock(void)
{
    m_traverse_mutex.lock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::traverseUnlock(void)
{
    m_traverse_mutex.unlock() ;
}
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
inline double
osgODE::World::getSimulationTime(void) const
{
    return m_simulation_time ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::World::setSimulationTime(double time)
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
osgODE::World::setWindFrequency(double Hz)
{
    m_wind_frequency = Hz ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
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
osgODE::World::setAirDensity(double density)
{
    m_air_density = density ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
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
inline double
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
osgODE::World::setERP(double value)
{
    dWorldSetERP(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline double
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
osgODE::World::setCFM(double value)
{
    dWorldSetCFM(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline double
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
osgODE::World::setQuickStepW(double value)
{
    dWorldSetQuickStepW(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline double
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
osgODE::World::setContactMaxCorrectingVel(double value)
{
    dWorldSetContactMaxCorrectingVel(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline double
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
osgODE::World::setContactSurfaceLayer(double value)
{
    dWorldSetContactSurfaceLayer(m_ODE_world, value) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// Auto generated
/* ....................................................................... */
inline double
osgODE::World::getContactSurfaceLayer(void) const
{
    return dWorldGetContactSurfaceLayer(m_ODE_world) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_WORLD_INL */
