#ifndef USE_PVIEWER
#define USE_PVIEWER 0
#endif


#include "Car"

#include <osgDB/ReadFile>

#include <osgODE/Notify>
#include <osgODE/CollisionParameters>
#include <osgODE/DefaultNearCallback>
#include <osgODE/Manager>
#include <osgODE/RigidBody>
#include <osgODE/Space>

#include <osgODEUtil/ControllerBase>
#include <osgODEUtil/MatrixManipulator>

#if USE_PVIEWER
    #include <pViewer/Viewer>
    #include <pViewer/RenderPasses>
    #include <pViewer/Light>
    #include <osgViewer/ViewerEventHandlers>
#else
    #include <osgViewer/Viewer>
    #include <osgViewer/ViewerEventHandlers>
    #include <osg/LightSource>
    #include <osg/Light>
#endif




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif




using namespace osgODE ;
using namespace osgODEUtil ;



// Get a directional light
osg::Group* create_light(void) ;




int
main(int argc, char** argv)
{
//     psLogLevel = LOG_LEVEL_ALL ;


    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH "/osgodecar" ) ;


    // The graph
    osg::Group*     root = create_light() ;



    //
    // create and configure the manager
    //

    osg::ref_ptr<Manager>   manager = new Manager() ;

    root->addChild( manager ) ;

    // The space
    Space*  space = new Space() ;
    manager->setWorld( space ) ;

    // quick setup
    manager->setup( false,      // threaded
                    true,       // accept visitors
                    1.0/121.0   // step size
                  ) ;



    //
    // Create a system with hard contacts (high ERP, low CFM)
    //
    CollisionParameters*            cp = new CollisionParameters() ;
    osgODE::DefaultNearCallback*    near_cbk = new osgODE::DefaultNearCallback() ;

    near_cbk->setCollisionParameters(cp) ;
    space->setNearCallback( near_cbk ) ;

    cp->setMode( dContactApprox1 | dContactSoftERP | dContactSoftCFM | dContactFDir1 /*| dContactMu2*/ ) ;

    cp->setSoftERP(0.8) ;
    cp->setSoftCFM(1.0e-5) ;
    cp->setMu(0.5) ;
//     cp->setMu2(0.75) ;



    // The world is quite responsive
    space->setERP(0.9) ;
    space->setCFM(1.0e-5) ;




    // the ground:

    osg::ref_ptr<ODEObject> ground = dynamic_cast<ODEObject*>( osgDB::readObjectFile("Ground.osgb") ) ;
    PS_ASSERT1( ground.valid() ) ;

    space->addObject(ground) ;

    // reset any local collision parameters
    ground->asCollidable()->setCollisionParameters(NULL) ;




    // the car:
    Car*    car = new Car() ;
    car->init() ;

    space->addObject( car ) ;




    //
    // The viewer. The pViewer package will not be released soon
    //

#if USE_PVIEWER

    bool    HDR = false ;

    osg::ArgumentParser arguments(&argc,argv) ;

    while( arguments.read("--hdr") ) {
        HDR = true ;
    }



    pViewer::Viewer     viewer ;
    viewer.setEnableFullScreenEffects(true) ;
//     viewer.setRenderPass( pViewer::createLo() ) ;
    viewer.setRenderPass( pViewer::createHi(HDR, HDR, "../../../pViewer/examples/data/cube") ) ;

#else

    osgViewer::Viewer   viewer ;
#endif

    viewer.setSceneData(root) ;



    // the controller

    ControllerBase* controller = new ControllerBase() ;

    controller->onKeyPressed()->connect( car, &Car::handleKeyDown ) ;
    controller->onKeyReleased()->connect( car, &Car::handleKeyUp ) ;

    viewer.addEventHandler( controller ) ;



    //
    // the camera manipulator:
    //

    car->getBody()->setCameraManipulator( new osgODEUtil::MatrixManipulator() ) ;

    // the eye is behind the car
    car->getBody()->setCameraManipulatorCenter( osg::Vec3(0.0, -7.5, 1.0) ) ;

    // The camera looks toward the local Y axis
    car->getBody()->setCameraManipulatorDirection( osg::Y_AXIS ) ;

    // The "up" axis is the global Z
    car->getBody()->setCameraManipulatorUp( osg::Z_AXIS ) ;
    car->getBody()->setCameraManipulatorUpObjectSpace( false ) ;

    // soft motion
    car->getBody()->setCameraManipulatorElasticity( 10 ) ;

    viewer.setCameraManipulator( car->getBody()->getCameraManipulator() ) ;



    osgViewer::StatsHandler*    stats = new osgViewer::StatsHandler() ;
    stats->setKeyEventTogglesOnScreenStats('0') ;

    viewer.addEventHandler(stats) ;





    // Projection matrix

    osg::GraphicsContext::ScreenIdentifier  si ;
    si.readDISPLAY() ;
    si.setUndefinedScreenDetailsToDefaultScreen() ;
    osg::GraphicsContext::ScreenSettings    settings ;
    osg::GraphicsContext::getWindowingSystemInterface()->getScreenSettings(si, settings) ;

    viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, (double)settings.width / (double)settings.height, 0.1, 1000.0) ;



    // enjoy

    viewer.run() ;



    return 0 ;
}




osg::Group*
create_light(void)
{
#if USE_PVIEWER
    osg::Group*         group = new osg::Group() ;


    {
        pViewer::Light* light = new pViewer::Light() ;

        osg::Vec3   position = osg::Vec3(1, -2, 2) ;
        position.normalize() ;

        light->setPosition( osg::Vec4(position, 0.0) ) ;
        light->setDirection( osg::Vec4(-position, 0.0) ) ;
        light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0) ) ;
        light->setDiffuse( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;
        light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

        light->setShadowEnabled(true) ;
        light->setShadowBufferSize(1024) ;
        light->setShadowBias(3.0) ;
        light->setShadowFrustumSize(15) ;

        light->setType( pViewer::Light::DIRECTIONAL ) ;

        group->addChild(light) ;
    }



#else
    osg::LightSource*   group = new osg::LightSource() ;


    {
        osg::Light* light = new osg::Light() ;

        osg::Vec3   position = osg::Vec3(1, -2, 2) ;
        position.normalize() ;

        light->setLightNum(0) ;
        light->setPosition( osg::Vec4(position, 0.0) ) ;
        light->setDirection( -position ) ;
        light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0) ) ;
        light->setDiffuse( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;
        light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

        group->setLight(light) ;
    }
#endif



    return group ;
}
