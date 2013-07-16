/*!
 * @file AnimationHelper.cpp
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
#include <osgODEUtil/AnimationHelper>
#include <osgODEUtil/AnimationManager>

#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
AnimationHelper::AnimationHelper(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationHelper::AnimationHelper(const AnimationHelper& other, const osg::CopyOp& copyop):
    osgODE::ODECallback         ( other, copyop ),
    m_animation_manager_list    ( other.m_animation_manager_list )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationHelper::~AnimationHelper(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::operator()( osgODE::ODEObject* object )
{
    traverse( object ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    class OSG_EXPORT AnimationManagerCollector: public osg::NodeVisitor
    {
    public:
            AnimationManagerCollector(void): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}


        virtual void    apply( osg::Node& n )
        {
            osg::NodeCallback*  nc = n.getUpdateCallback() ;

            while( nc != NULL ) {
                AnimationManager*   am = dynamic_cast<AnimationManager*>( nc ) ;

                if( am ) {
                    m_ams.push_back( am ) ;
                }

                nc = nc->getNestedCallback() ;
            }
        }


        AnimationHelper::AnimationManagerList&   getAnimationManagerList(void)
                    { return m_ams ; }


    private:
        AnimationHelper::AnimationManagerList    m_ams ;
    } ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::collectAnimationManagers( osg::Node* graph )
{
    osg::ref_ptr< AnimationManagerCollector >   nv = new AnimationManagerCollector() ;
    graph->accept(*nv) ;

    setAnimationManagerList( nv->getAnimationManagerList() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::play( const std::string& name, float weight, float fadein )
{
    for( unsigned int i=0; i<m_animation_manager_list.size(); i++ ) {
        m_animation_manager_list[i]->play( name, weight, fadein ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::stop( const std::string& name )
{
    for( unsigned int i=0; i<m_animation_manager_list.size(); i++ ) {
        m_animation_manager_list[i]->stop( name ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
AnimationHelper::isPlaying( const std::string& name )
{
    for( unsigned int i=0; i<m_animation_manager_list.size(); i++ ) {
        if( m_animation_manager_list[i]->isPlaying( name ) ) {
            return true ;
        }
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */
