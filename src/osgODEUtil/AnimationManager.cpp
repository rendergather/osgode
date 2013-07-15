/*!
 * @file AnimationManager.cpp
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
#include <osgODEUtil/AnimationManager>
#include <osgODEUtil/HashTable>

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
AnimationManager::AnimationManager(void):
    m_animation_table   ( new HashTable() )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationManager::AnimationManager(const AnimationManager& other, const osg::CopyOp& copyop):
    osgAnimation::BasicAnimationManager ( other, copyop ),
    m_animation_table                   ( osg::clone( other.m_animation_table.get(), copyop ) )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationManager::AnimationManager(const osgAnimation::BasicAnimationManager& other, const osg::CopyOp& copyop):
    osgAnimation::BasicAnimationManager ( other, copyop ),
    m_animation_table                   ( new HashTable() )
{

    for( unsigned  int i=0; i<_animations.size(); i++ ) {
        m_animation_table->set( _animations[i]->getName(), _animations[i] ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AnimationManager::~AnimationManager(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationManager::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    this->osgAnimation::BasicAnimationManager::operator()(node, nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationManager::play(const std::string& name, float weight, float fadein, int priority)
{
    osgAnimation::Animation*    animation = dynamic_cast<osgAnimation::Animation*>( m_animation_table->get(name) ) ;

    PS_ASSERT1( animation != NULL ) ;

    if( animation ) {
        this->osgAnimation::BasicAnimationManager::playAnimation( animation, priority, weight ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationManager::stop(const std::string& name, float fadeout)
{
    osgAnimation::Animation*    animation = dynamic_cast<osgAnimation::Animation*>( m_animation_table->get(name) ) ;

    PS_ASSERT1( animation != NULL ) ;

    if( animation ) {
        this->osgAnimation::BasicAnimationManager::stopAnimation( animation ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationManager::registerAnimation( osgAnimation::Animation* animation )
{
    PS_ASSERT1( animation != NULL ) ;



    const std::string&  ani_name = animation->getName() ;

    PS_ASSERT1( ! m_animation_table->get( ani_name ) ) ;


    this->osgAnimation::BasicAnimationManager::registerAnimation( animation ) ;

    m_animation_table->set( ani_name, animation ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AnimationManager::unregisterAnimation( osgAnimation::Animation* animation )
{
    PS_ASSERT1( animation != NULL ) ;


    this->osgAnimation::BasicAnimationManager::unregisterAnimation( animation ) ;

    m_animation_table->reset( animation->getName() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgAnimation::Animation*
AnimationManager::getAnimation( const std::string& name )
{
    return dynamic_cast<osgAnimation::Animation*>( m_animation_table->get( name ) ) ;
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
            osg::NodeCallback*  nc = n.getUpdateCallback() ;

            while( nc != NULL ) {
                osgAnimation::BasicAnimationManager*    bam = dynamic_cast<osgAnimation::BasicAnimationManager*>( nc ) ;

                if( bam ) {
                    m_bams.push_back( Bam(&n, bam) ) ;
                }

                nc = nc->getNestedCallback() ;
            }



            n.setCullingActive(false) ;
            traverse(n) ;
        }


        class Bam
        {
        public:
            Bam(osg::Node* n, osgAnimation::BasicAnimationManager* bam): mNode(n), mBam(bam) {}
            osg::ref_ptr<osg::Node>                             mNode ;
            osg::ref_ptr<osgAnimation::BasicAnimationManager>   mBam ;
        } ;

        typedef std::vector< Bam >  BamList ;

        BamList&    getBamList(void) { return m_bams ; }


    private:
        BamList     m_bams ;
    } ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
AnimationManager::replaceBasicAnimationManagers( osg::Node* graph )
{
    osg::ref_ptr< BasicAnimationManagerFinder > nv = new BasicAnimationManagerFinder() ;
    graph->accept(*nv) ;


    BasicAnimationManagerFinder::BamList&   bams = nv->getBamList() ;

    for( unsigned int i=0; i<bams.size(); i++ ) {
        const BasicAnimationManagerFinder::Bam&     bam = bams[i] ;

        AnimationManager*    am = new AnimationManager( *(bam.mBam) ) ;

        bam.mNode->removeUpdateCallback( bam.mBam ) ;
        bam.mNode->addUpdateCallback( am ) ;
    }


    return bams.size() ;
}
/* ....................................................................... */
/* ======================================================================= */
