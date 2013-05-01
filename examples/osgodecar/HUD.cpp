/*!
 * @file HUD.cpp
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
// #include <osgODE/HUD>
#include "HUD"
#include "Car"
#include "Text"

#include <osgODE/RigidBody>
#include <osgODE/Engine>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/PolygonOffset>

#include <osgDB/ReadFile>

#include <osgODE/Notify>


#if USE_PVIEWER
    #include <pViewer/NodeMasks>
#endif
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
HUD::HUD(Car* car):
    m_car(car)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HUD::HUD(const HUD& other, const osg::CopyOp& copyop):
    osg::Camera(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HUD::~HUD(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
HUD::traverse(osg::NodeVisitor& nv)
{
    if( nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {

        {
            const double    speed = m_car->getBody()->getLinearVelocity().length() ;
            const double    angle = 0.5 * osg::PI - osg::PI * speed * (3.6 / 240.0) ;

            m_speed_hand->setAttitude( osg::Quat( angle, osg::Z_AXIS ) ) ;
        }


        {
            const double    speed = m_car->getEngine()->getSpeed() ;
            const double    rpm = 60.0 * speed / (2.0 * osg::PI) ;
            const double    angle = 0.5 * osg::PI - osg::PI * rpm / 9000 ;

            m_rpm_hand->setAttitude( osg::Quat( angle, osg::Z_AXIS ) ) ;
        }


        {
            const unsigned int  gear = m_car->getEngine()->getCurrentGear() ;

            if( gear > 1 ) {
                char    buf[2] ;
                buf[0] = '0' + gear - 1 ;
                buf[1] = 0 ;
                m_gear_text->setText( buf ) ;
            }
            else if( gear == 0 ) {
                m_gear_text->setText( "R" ) ;
            }
            else {
                m_gear_text->setText("N") ;
            }
        }


    }


    this->Camera::traverse( nv ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
HUD::init(double aspect)
{
    removeChildren( 0, getNumChildren() ) ;


    setRenderOrder( NESTED_RENDER ) ;

    setProjectionMatrixAsOrtho2D( -aspect, aspect, -1, 1 ) ;




    osg::StateSet*  state_set = getOrCreateStateSet() ;

    state_set->setMode( GL_LIGHTING, osg::StateAttribute::OFF ) ;


    setReferenceFrame( osg::Transform::ABSOLUTE_RF ) ;
    setViewMatrix( osg::Matrix::identity() ) ;


    osg::Node*  static_objects = _createStaticObjects() ;
    osg::Node*  dynamic_objects = _createDynamicObjects() ;


    addChild( static_objects ) ;
    addChild( dynamic_objects ) ;


    static_objects->getStateSet()->setRenderBinDetails(1, "RenderBin") ;
    dynamic_objects->getStateSet()->setRenderBinDetails(2, "RenderBin") ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Node*
HUD::_createStaticObjects(void)
{
    osg::Geode* static_objects = new osg::Geode() ;



    osg::Drawable*  rpm_quad = osg::createTexturedQuadGeometry( osg::Vec3(-1.25, -0.9, 0),
                                                                osg::Vec3(1, 0, 0),
                                                                osg::Vec3(0, 1, 0)
                                                              ) ;



    osg::Drawable*  speed_quad = osg::createTexturedQuadGeometry(   osg::Vec3(0.25, -0.9, 0),
                                                                    osg::Vec3(1, 0, 0),
                                                                    osg::Vec3(0, 1, 0)
                                                                ) ;



    static_objects->addDrawable( rpm_quad ) ;
    static_objects->addDrawable( speed_quad ) ;






    osg::StateSet*  state_set = static_objects->getOrCreateStateSet() ;

    state_set->setMode(GL_BLEND, osg::StateAttribute::ON) ;






    {
        osg::Image* image = osgDB::readImageFile("car_rpm.png") ;

        PS_ASSERT1( image ) ;

        osg::Texture2D* texture = new osg::Texture2D( image ) ;

        rpm_quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON) ;
    }




    {
        osg::Image* image = osgDB::readImageFile("car_speed.png") ;

        PS_ASSERT1( image ) ;

        osg::Texture2D* texture = new osg::Texture2D( image ) ;

        speed_quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON) ;
    }



#if USE_PVIEWER
    {
        static_objects->setNodeMask( pViewer::NodeMasks::TRANSPARENCY ) ;


        rpm_quad->getOrCreateStateSet()->getOrCreateUniform("uColor", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(1,1,1,1) ) ;
        rpm_quad->getOrCreateStateSet()->getOrCreateUniform("uIOR", osg::Uniform::FLOAT)->set( 1.0f ) ;
    }
#endif



    return static_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Node*
HUD::_createDynamicObjects(void)
{
    osg::Group* dynamic_objects = new osg::Group() ;



    m_speed_hand = _createHand() ;
    dynamic_objects->addChild( m_speed_hand ) ;

    m_speed_hand->setPosition( osg::Vec3(0.75, -0.9, 0) ) ;



    m_rpm_hand = _createHand() ;
    dynamic_objects->addChild( m_rpm_hand ) ;

    m_rpm_hand->setPosition( osg::Vec3(-0.75, -0.9, 0) ) ;




    osg::StateSet*  state_set = dynamic_objects->getOrCreateStateSet() ;

    state_set->setMode(GL_BLEND, osg::StateAttribute::ON) ;


    // always on top
    state_set->setAttributeAndModes( new osg::PolygonOffset(0, -1) ) ;





    {
        m_gear_text  = new Text() ;

        m_gear_text->setText( " " ) ;


        osg::Image* image = osgDB::readImageFile("alphadigit.png") ;

        PS_ASSERT1( image ) ;

        osg::Texture2D* texture = new osg::Texture2D( image ) ;

        m_gear_text->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON) ;


        osg::Geode* geode = new osg::Geode() ;
        geode->addDrawable( m_gear_text ) ;

        dynamic_objects->addChild( geode ) ;


        m_gear_text->setScale( 1.0/3.0 ) ;

        m_gear_text->setPosition( osg::Vec2( -0.5 * m_gear_text->getScale(), -0.9  ) ) ;
    }



#if USE_PVIEWER
    {
        dynamic_objects->setNodeMask( pViewer::NodeMasks::TRANSPARENCY ) ;
        dynamic_objects->getOrCreateStateSet()->getOrCreateUniform("uColor", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(1,1,1,1) ) ;
        dynamic_objects->getOrCreateStateSet()->getOrCreateUniform("uIOR", osg::Uniform::FLOAT)->set( 1.0f ) ;
    }
#endif



    return dynamic_objects ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::PositionAttitudeTransform*
HUD::_createHand(void)
{
    osg::Drawable*  quad = osg::createTexturedQuadGeometry( osg::Vec3(-1.0/32.0, -1.0/16, 0),
                                                            osg::Vec3(1.0/16.0, 0, 0),
                                                            osg::Vec3(0, 1, 0)
                                                          ) ;


    osg::Geode* geode = new osg::Geode() ;

    geode->addDrawable( quad ) ;



    {
        osg::Image* image = osgDB::readImageFile("hud_hand.png") ;

        PS_ASSERT1( image ) ;

        osg::Texture2D* texture = new osg::Texture2D( image ) ;

        quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON) ;
    }




    osg::PositionAttitudeTransform*   mt = new osg::PositionAttitudeTransform() ;
    mt->addChild( geode ) ;


    mt->setScale( osg::Vec3(0.5, 0.5, 1.0) ) ;


    mt->setAttitude( osg::Quat( osg::PI * 0.5, osg::Z_AXIS ) ) ;

    return mt ;
}
/* ....................................................................... */
/* ======================================================================= */
