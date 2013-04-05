/*!
 * @file DynamicParticleSystem.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/DynamicParticleSystem>
#include <osgODE/CommonWorldOperations>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleSystem::DynamicParticleSystem(World* world):
    m_world(world)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleSystem::DynamicParticleSystem(const DynamicParticleSystem& other, const osg::CopyOp& copyop):
    osgParticle::ParticleSystem(other, copyop),
    m_body_list( other.m_body_list ),
    m_world( other.m_world ),
    m_body_template( other.m_body_template )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleSystem::~DynamicParticleSystem(void)
{
    if( m_world.valid() ) {

        for(unsigned int i=0; i<m_body_list.size(); i++) {
            RigidBody*  body = m_body_list[i].get() ;

            if( body ) {
                m_world->addOperation( new RemoveObjectOperation( body ) ) ;
            }
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgParticle::Particle*
DynamicParticleSystem::createParticle( const osgParticle::Particle* ptemplate )
{
    // is there any dead particle?
    if (!_deadparts.empty()) {

        // retrieve a pointer to the last dead particle
        osgParticle::Particle* P = _deadparts.top();

        // create a new (alive) particle in the same place
        *P = ptemplate? *ptemplate: _def_ptemp;

        // remove the pointer from the death stack
        _deadparts.pop();
        return P;

    } else {

        // add a new particle to the vector
        _particles.push_back(ptemplate? *ptemplate: _def_ptemp);



        if( m_world.valid() && m_body_template.valid() ) {
            RigidBody*  body = osg::clone( m_body_template.get() ) ;

            body->setBodyEnabled( false ) ;

            m_world->addOperation( new AddObjectOperation( body ) ) ;

            m_body_list.push_back( body ) ;
        }


        
        return &_particles.back();
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DynamicParticleSystem::destroyParticle( int i )
{
    RigidBody*  body = getBody(i) ;

    if( body ) {
        body->setBodyEnabled( false ) ;
    }


    this->osgParticle::ParticleSystem::destroyParticle( i ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DynamicParticleSystem::reuseParticle( int i )
{
    RigidBody*  body = getBody(i) ;

    if( body ) {
        body->setBodyEnabled( false ) ;
    }


    this->osgParticle::ParticleSystem::reuseParticle( i ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DynamicParticleSystem::update(double dt, osg::NodeVisitor& nv)
{
    for(unsigned int i=0; i<_particles.size(); ++i) {

        osgParticle::Particle&  particle = _particles[i] ;

        RigidBody*              body = getBody(i) ;

        if( ! body ) {
            continue ;
        }

        if( particle.isAlive() && ! body->getBodyEnabled() ) {

            body->setPosition( particle.getPosition() ) ;
            body->setLinearVelocity( particle.getVelocity() ) ;
            body->setAngularVelocity( particle.getAngularVelocity() ) ;
            body->setBodyEnabled( true ) ;

        } else {

            particle.setPosition( body->getPosition() ) ;
            particle.setVelocity( body->getLinearVelocity() ) ;
            particle.setAngularVelocity( body->getAngularVelocity() ) ;

        }
    }



    this->osgParticle::ParticleSystem::update(dt, nv) ;
}
/* ....................................................................... */
/* ======================================================================= */
