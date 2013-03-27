/*!
 * @file Sensor.cpp
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Sensor>
#include <osgODE/World>
#include <osgODE/Notify>

#include <iostream>
#include <osg/io_utils>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Sensor::Sensor(void):
    m_radius(0.0)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Sensor::Sensor(const Sensor& other, const osg::CopyOp& copyop):
    Transformable(other, copyop),
    m_position( other.m_position ),
    m_quaternion( other.m_quaternion ),
    m_radius( other.m_radius ),
    m_callback( other.m_callback )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Sensor::~Sensor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
namespace {
    class Interactor: public World::InteractionCallback
    {
    public:
        virtual void    operator()(ODEObject* object, ODEObject*)
        {   m_objects.push_back(object) ;   }

        World::Objects& getObjects(void)
        {   return m_objects ;  }

    private:
        World::Objects m_objects ;

    } ;
}
/* ....................................................................... */
void
Sensor::postUpdate(double step_size)
{

    {
        osg::Matrix     m = osg::Matrix::rotate( m_quaternion ) * osg::Matrix::translate( m_position ) ;

        this->Transformable::getMatrixTransform()->setMatrix( m ) ;
    }



    World*  world = getWorld() ;

    PS_ASSERT1( world != NULL ) ;

    osg::BoundingSphere bs ;

    bs.set( m_position, m_radius ) ;


    osg::ref_ptr<Interactor> interactor = new Interactor() ;

    world->findInteractions( interactor.get(), bs ) ;


    if( world->findInteractions( interactor.get(), bs)  ) {

        interaction() ;


        if( m_callback.valid()  ) {

            (*m_callback)(this) ;
        }

    }


    this->ODEObject::postUpdate( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Sensor::interaction(void)
{
}
/* ....................................................................... */
/* ======================================================================= */
