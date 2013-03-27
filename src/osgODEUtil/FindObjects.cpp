/*!
 * @file FindObjects.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
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
#include <osgODEUtil/FindObjects>

#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/Joint>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace
{
    class FindManagerVisitor: public osg::NodeVisitor
    {
    public:
        FindManagerVisitor(void):
            osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}


        ~FindManagerVisitor(void) {}


        virtual void    apply(osg::Node& node)
        {
            if( ! m_manager.valid() ) {
                m_manager = dynamic_cast<osgODE::Manager*>( &node ) ;
            }

            if( ! m_manager.valid() ) {
                traverse(node) ;
            }
        }


        osgODE::Manager*    getManager(void)
        {
            return m_manager.get() ;
        }

    private:
        osg::ref_ptr<osgODE::Manager>   m_manager ;
    } ;




    class FindNodeVisitor: public osg::NodeVisitor
    {
    public:
        FindNodeVisitor(const std::string& name):
            osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
            m_name(name) {}


        ~FindNodeVisitor(void) {}


        virtual void    apply(osg::Node& node)
        {
            if( ( ! m_node.valid() )  && (node.getName() == m_name) ) {
                m_node = &node ;
            } else {
                traverse(node) ;
            }
        }


        osg::Node*  getNode(void)
        {
            return m_node.get() ;
        }

    private:
        osg::ref_ptr<osg::Node> m_node ;
        std::string             m_name ;
    } ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Node*
osgODEUtil::findNode( osg::Node* graph, const std::string& name )
{
    FindNodeVisitor v(name) ;

    graph->accept(v) ;

    return v.getNode() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::Manager*
osgODEUtil::findManager( osg::Node* graph )
{
    FindManagerVisitor  v ;

    graph->accept(v) ;

    return v.getManager() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
osgODEUtil::findJoints(osgODE::World* world, Joints& joints, const std::string& regex)
{


    osgODE::World::Objects  objects ;

    world->getObjectsByRegexName(regex, objects) ;

    osgODE::World::Objects::iterator    itr = objects.begin() ;
    osgODE::World::Objects::iterator    itr_end = objects.end() ;

    while( itr != itr_end ) {
        osgODE::Joint*  joint = (*itr++)->asJoint() ;

        if( joint ) {
            joints.push_back(joint) ;
        }
    }


    return joints.size() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
osgODEUtil::findRigidBodies(osgODE::World* world, RigidBodies& bodies, const std::string& regex)
{


    osgODE::World::Objects  objects ;

    world->getObjectsByRegexName(regex, objects) ;

    osgODE::World::Objects::iterator    itr = objects.begin() ;
    osgODE::World::Objects::iterator    itr_end = objects.end() ;

    while( itr != itr_end ) {
        osgODE::RigidBody*  body = (*itr++)->asRigidBody() ;

        if( body ) {
            bodies.push_back(body) ;
        }
    }


    return bodies.size() ;
}
/* ....................................................................... */
/* ======================================================================= */
