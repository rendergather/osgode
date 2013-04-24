#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/TriMesh>
#include <osgODE/AMotorJoint>
#include <osgODE/LMotorJoint>
#include <osgODE/Notify>

#include <osgODEUtil/CreateTriMeshFromNode>
#include <osgODEUtil/MatrixManipulator>
#include <osgODEUtil/ControllerBase>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "CharacterEx"



#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/export"
#endif





osg::Group* create_light() ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::Group* root = create_light() ;


//     psLogLevel = LOG_LEVEL_ALL ;




    /*
     * [1] Create the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;

    root->addChild( manager ) ;

    manager->setup(false, true, 1.0/100.0) ;

    manager->setWorld( new osgODE::Space() ) ;




    /*
     * [2] Create a kinematic plane as floor
     */
    {
        osg::ref_ptr<osgODE::Collidable>    ground = dynamic_cast<osgODE::Collidable*>( osgDB::readObjectFile("large_ground.osgb") ) ;
        PS_ASSERT1( ground.valid() ) ;

        manager->getWorld()->addObject(ground) ;

        // reset any local collision parameters
        ground->setCollisionParameters(NULL) ;
    }




    /*
     * [3] Create the character
     */
    osgODE::CharacterEx*    character = new osgODE::CharacterEx() ;
    {

        character->init() ;

        character->getBody()->setPosition( osg::Z_AXIS * 10 ) ;

        manager->getWorld()->addObject( character ) ;
    }




    /*
     * [4] setup the view
     */

    osgViewer::Viewer   viewer ;


    //
    // the controller
    //
    osgODEUtil::ControllerBase* controller = new osgODEUtil::ControllerBase() ;
    {
        viewer.addEventHandler( controller ) ;


        controller->onKeyPressed()  ->connect( character, &osgODE::CharacterEx::handleKeyDown   ) ;
        controller->onKeyReleased() ->connect( character, &osgODE::CharacterEx::handleKeyUp     ) ;
        controller->onMouseMoved()  ->connect( character, &osgODE::CharacterEx::handleMove      ) ;
        controller->onFrame()       ->connect( character, &osgODE::CharacterEx::handleFrame     ) ;


        character->setSensitivity( 0.25 ) ;
    }



    character->getBody()->setCameraManipulator( new osgODEUtil::MatrixManipulator() ) ;
    character->getBody()->setCameraManipulatorCenter( osg::Vec3(0.0, 0.0, 0.0) ) ;
    character->getBody()->setCameraManipulatorDirection( -osg::Z_AXIS) ;
    character->getBody()->setCameraManipulatorUp( osg::Y_AXIS ) ;
    character->getBody()->setCameraManipulatorUpObjectSpace( true ) ;
    character->getBody()->setCameraManipulatorElasticity( 0 ) ;

    viewer.setCameraManipulator( character->getBody()->getCameraManipulator() ) ;



    osgViewer::StatsHandler*    stats = new osgViewer::StatsHandler() ;
    stats->setKeyEventTogglesOnScreenStats('0') ;

    viewer.addEventHandler(stats) ;



    viewer.setSceneData( root ) ;

    return viewer.run() ;
}




osg::Group*
create_light(void)
{
    osg::Group* group = NULL ;



    {
        osg::LightSource*   ls = new osg::LightSource() ;
        osg::Light* light = new osg::Light() ;

        osg::Vec3   position = osg::Vec3(1, -2, 2) ;
        position.normalize() ;

        light->setLightNum(0) ;
        light->setPosition( osg::Vec4(position, 0.0) ) ;
        light->setDirection( -position ) ;
        light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0) ) ;
        light->setDiffuse( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;
        light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

        ls->setLight(light) ;

        group = ls ;
    }



    return group ;
}
