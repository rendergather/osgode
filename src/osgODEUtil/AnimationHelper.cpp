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

#include <osgODE/ODEObject>
#include <osgODE/Notify>

#include <osgAnimation/BasicAnimationManager>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
AnimationHelper::AnimationHelper(void):
    m_dirty     ( true )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationHelper::AnimationHelper(const AnimationHelper& other, const osg::CopyOp& copyop):
    osgODE::ODECallback ( other, copyop ),
    m_dirty             ( true )
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
    PS_ASSERT1( object ) ;

    if( m_dirty ) {
        init( object ) ;
        m_dirty = false ;
    }


    update( object ) ;


    traverse( object ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::update( osgODE::ODEObject* object )
{
	(void) object ;

    osgAnimation::Animation*    idle = m_animation_map["IDLE"] ;


    if( idle ) {

        if( ! m_bam->isPlaying(idle) ) {
            m_bam->playAnimation(idle) ;
        }

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationHelper::init( osgODE::ODEObject* object )
{
    m_bam = _findBasicAnimationManager( object ) ;

    PS_ASSERT1( m_bam.valid() ) ;


    m_animation_map = _buildAnimationMap( m_bam.get() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const AnimationHelper::AnimationMap
AnimationHelper::_buildAnimationMap( osgAnimation::BasicAnimationManager* bam )
{
    AnimationMap    animation_map ;

    for( unsigned int i = 0; i < bam->getAnimationList().size(); i++ ) {

        osgAnimation::Animation*    current = bam->getAnimationList()[i] ;

        animation_map[ current->getName() ] = current ;

        current->setPlayMode( osgAnimation::Animation::STAY ) ;
    }


    return animation_map ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    class OSG_EXPORT BasicAnimationManagerFinder: public osg::NodeVisitor
    {
    public:
            BasicAnimationManagerFinder(void): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}


        virtual void    apply( osg::Node& n )
        {
            if( ! m_bam.valid() ) {
                osg::NodeCallback*  nc = n.getUpdateCallback() ;

                while( nc != NULL ) {
                    osgAnimation::BasicAnimationManager*    bam = dynamic_cast<osgAnimation::BasicAnimationManager*>( nc ) ;

                    if( bam ) {
                        m_bam = bam ;
                    }

                    nc = nc->getNestedCallback() ;
                }


            }

            n.setCullingActive(false) ;
                traverse(n) ;
        }


        osgAnimation::BasicAnimationManager*    getBAM(void)
        {   return m_bam.get() ; }


    private:
        osg::ref_ptr<osgAnimation::BasicAnimationManager>   m_bam ;
    } ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgAnimation::BasicAnimationManager*
AnimationHelper::_findBasicAnimationManager( osgODE::ODEObject* object )
{
    osg::ref_ptr<BasicAnimationManagerFinder>   visitor = new BasicAnimationManagerFinder() ;

    object->accept( *visitor ) ;


    return visitor->getBAM() ;
}
/* ....................................................................... */
/* ======================================================================= */
