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

#include <osg/Geode>
#include <osg/Texture2D>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace
{
    class FindManagersVisitor: public osg::NodeVisitor
    {
    public:
        FindManagersVisitor(osgODEUtil::Managers& managers):
            osg::NodeVisitor    ( TRAVERSE_ALL_CHILDREN ),
            m_managers          ( managers ) {}


        virtual ~FindManagersVisitor(void) {}


        virtual void    reset(void)
        {
            m_managers.clear() ;
        }


        virtual void    apply(osg::Node& node)
        {
            osgODE::Manager*    manager = dynamic_cast<osgODE::Manager*>( &node ) ;

            if( manager != NULL ) {
                m_managers.push_back( manager ) ;
            }

            traverse( node ) ;
        }

    private:
        osgODEUtil::Managers&   m_managers ;
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




    class FindTexture2DVisitor: public osg::NodeVisitor
    {
    public:
        FindTexture2DVisitor(const std::string& name):
            osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
            m_name(name) {}


        ~FindTexture2DVisitor(void) {}


        virtual void    apply(osg::Node& node)
        {
            if( m_texture.valid() ) {
                return ;
            }


            if( node.getStateSet() ) {
                apply( *node.getStateSet() ) ;
            }

            traverse(node) ;
        }


        virtual void    apply(osg::Geode& geode)
        {
            if( m_texture.valid() ) {
                return ;
            }

            if( geode.getStateSet() ) {
                apply( *geode.getStateSet() ) ;
            }

            for( unsigned int i=0; i<geode.getNumDrawables(); i++ ) {
                osg::Drawable*  drawable = geode.getDrawable(i) ;

                if( drawable ) {
                    if( drawable->getStateSet() ) {
                        apply( *drawable->getStateSet() ) ;
                    }
                }
            }

            traverse(geode) ;
        }


        virtual void    apply(osg::StateSet& stateset)
        {
            for( unsigned int i=0; i<stateset.getTextureAttributeList().size(); i++ ) {
                osg::Texture2D* texture = dynamic_cast<osg::Texture2D*>( stateset.getTextureAttribute(i, osg::StateAttribute::TEXTURE) ) ;

                if (texture) {
                    if( texture->getName() == m_name ) {
                        m_texture = texture ;
                        break ;
                    }
                }
            }
        }


        osg::Texture2D* getTexture2D(void)
        {
            return m_texture.get() ;
        }

    private:
        osg::ref_ptr<osg::Texture2D>    m_texture ;
        std::string                     m_name ;
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
osg::Texture2D*
osgODEUtil::findTexture2D( osg::Node* graph, const std::string& name )
{
    FindTexture2DVisitor    v(name) ;

    graph->accept(v) ;

    return v.getTexture2D() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::Manager*
osgODEUtil::findManager( osg::Node* graph )
{
    Managers    managers ;

    if( findManagers( graph, managers ) ) {
        return managers[0] ;
    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
osgODEUtil::findManagers( osg::Node* graph, osgODEUtil::Managers& managers )
{
    FindManagersVisitor nv(managers) ;

    graph->accept(nv) ;

    return managers.size() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
osgODEUtil::findJoints(osgODE::World* world, Joints& joints, const std::string& regex)
{


    osgODE::World::ObjectList  objects ;

    world->getObjectsByRegexName(regex, objects) ;

    osgODE::World::ObjectList::iterator    itr = objects.begin() ;
    osgODE::World::ObjectList::iterator    itr_end = objects.end() ;

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


    osgODE::World::ObjectList  objects ;

    world->getObjectsByRegexName(regex, objects) ;

    osgODE::World::ObjectList::iterator    itr = objects.begin() ;
    osgODE::World::ObjectList::iterator    itr_end = objects.end() ;

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




/* ======================================================================= */
/* ....................................................................... */
unsigned int
osgODEUtil::findWorlds( osg::Node* graph, osgODEUtil::Worlds& worlds )
{
    Managers    ms ;

    findManagers( graph, ms ) ;


    for(unsigned int i=0; i<ms.size(); i++) {
        if( ms[i]->getWorld() ) {
            worlds.push_back( ms[i]->getWorld() ) ;
        }
    }

    return worlds.size() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::World*
osgODEUtil::findWorld( osg::Node* graph )
{
    Worlds  worlds ;

    if( findWorlds( graph, worlds) ) {
        return worlds[0] ;
    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */
