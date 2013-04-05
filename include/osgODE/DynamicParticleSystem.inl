/*!
 * @file DynamicParticleSystem.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _OSGODE_DYNAMICPARTICLESYSTEM_INL
#define _OSGODE_DYNAMICPARTICLESYSTEM_INL




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DynamicParticleSystem::setBodyList( const BodyList& body_list )
{
    m_body_list = body_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::DynamicParticleSystem::BodyList&
osgODE::DynamicParticleSystem::getBodyList(void)
{
    return m_body_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::DynamicParticleSystem::BodyList&
osgODE::DynamicParticleSystem::getBodyList(void) const
{
    return m_body_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DynamicParticleSystem::setWorld(World* world)
{
    m_world = world ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::World*
osgODE::DynamicParticleSystem::getWorld(void)
{
    return m_world.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::World*
osgODE::DynamicParticleSystem::getWorld(void) const
{
    return m_world.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::RigidBody*
osgODE::DynamicParticleSystem::getBody(int i)
{
    if( (unsigned int)i < m_body_list.size() ) {
        return m_body_list[i].get() ;
    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::RigidBody*
osgODE::DynamicParticleSystem::getBody(int i) const
{
    if( (unsigned int)i < m_body_list.size() ) {
        return m_body_list[i].get() ;
    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::DynamicParticleSystem::setBodyTemplate(RigidBody* body)
{
    m_body_template = body ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::RigidBody*
osgODE::DynamicParticleSystem::getBodyTemplate(void)
{
    return m_body_template.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::RigidBody*
osgODE::DynamicParticleSystem::getBodyTemplate(void) const
{
    return m_body_template.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_DYNAMICPARTICLESYSTEM_INL */
