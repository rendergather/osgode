/*!
 * @file JointBreakCallback.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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
#include <osgODE/JointBreakCallback>
#include <osgODE/Joint>
#include <osgODE/Notify>
#include <osgODE/World>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    class RemoveJointOperation: public osgODE::World::Operation
    {
    public:
        RemoveJointOperation(osgODE::Joint* joint = NULL) :
            m_joint(joint) {}


        virtual void    operator()(osgODE::World* world)
        {
            if( m_joint.valid() ) {
                world->removeObject( m_joint.get() ) ;
            }
        }

    private:
        osg::observer_ptr<osgODE::Joint>    m_joint ;
    } ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
JointBreakCallback::JointBreakCallback(ooReal force_limit, ooReal torque_limit):
    m_force_limit( force_limit ),
    m_torque_limit( torque_limit ),
    m_remove_on_break( true ),
    m_enabled( true )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
JointBreakCallback::JointBreakCallback(const JointBreakCallback& other, const osg::CopyOp& copyop):
    ODECallback(other, copyop),

    m_force_limit( other.m_force_limit ),
    m_torque_limit( other.m_torque_limit ),
    m_remove_on_break( other.m_remove_on_break ),
    m_enabled( other.m_enabled )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
JointBreakCallback::~JointBreakCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
JointBreakCallback::operator()(ODEObject* object)
{
    if( m_enabled ) {


        Joint*  joint = object->asJoint() ;

        PS_ASSERT1( joint != NULL ) ;


        const JointFeedback*    feedback = joint->getOrCreateJointFeedback() ;

        const ooReal    force = osg::maximum( feedback->getF1().length(), feedback->getF2().length() ) ;
        const ooReal    torque = osg::maximum( feedback->getT1().length(), feedback->getT2().length() ) ;



        if( force > m_force_limit  ||  torque > m_torque_limit ) {

            if( m_remove_on_break ) {

                World*  world = joint->getWorld() ;
                PS_ASSERT1( world != NULL ) ;

                world->addOperation( new RemoveJointOperation(joint) ) ;

            } else {
                joint->setJointEnabled(false) ;
            }

        }


    } // enabled




    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
