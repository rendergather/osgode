#ifndef USE_PVIEWER
#define USE_PVIEWER 0
#endif


#include "Car"
#include "HUD"

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
#define OSGODE_DATA_PATH "../../data/export"
#endif




using namespace osgODE ;
using namespace osgODEUtil ;



// Get a directional light
osg::Group* create_light(bool nofx) ;




int
main(int argc, char** argv)
{
//     psLogLevel = LOG_LEVEL_ALL ;


    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;




#if USE_PVIEWER

    osg::ArgumentParser arguments(&argc,argv) ;

    bool            HDR = false ;
    bool            BLOOM = false ;
    bool            LOW = false ;
    bool            NO_FX = false ;
    std::string     SKY_CUBE_PREFIX = "" ;


    while( arguments.read("--hdr") ) {
        HDR = true ;
        BLOOM = true ;
    }

    while( arguments.read("--bloom") ) {
        BLOOM = true ;
    }

    while( arguments.read("--low") ) {
        LOW = true ;
    }

    while( arguments.read("--no-fx") ) {
        NO_FX = true ;
    }

    while( arguments.read("--sky-cube-prefix", SKY_CUBE_PREFIX) ) ;



    // The graph
    osg::Group*     root = create_light( NO_FX ) ;



#else

    osg::Group*     root = create_light( true ) ;

#endif









    //
    // Screen aspect:
    //
    osg::GraphicsContext::ScreenIdentifier  si ;
    si.readDISPLAY() ;
    si.setUndefinedScreenDetailsToDefaultScreen() ;
    osg::GraphicsContext::ScreenSettings    settings ;
    osg::GraphicsContext::getWindowingSystemInterface()->getScreenSettings(si, settings) ;

    const double    screen_aspect = (double)settings.width / (double)settings.height ;








    //
    // create and configure the manager
    //
    osg::ref_ptr<Manager>   manager = new Manager() ;
    Space*                  space   = new Space() ;
    {

        root->addChild( manager ) ;


        // The space
        manager->setWorld( space ) ;


        // quick setup
        manager->setup( false,      // threaded
                        true,       // accept visitors
                        1.0/120.0   // step size
                      ) ;
    }








    //
    // Configure world and collision callback
    //
    {
        osgODE::DefaultNearCallback*    near_cbk = new osgODE::DefaultNearCallback() ;
        CollisionParameters*            cp = new CollisionParameters() ;


        near_cbk->setCollisionParameters(cp) ;
        space->setNearCallback( near_cbk ) ;



        cp->setMode( dContactApprox1 | dContactSoftERP/* | dContactSoftCFM*/ | dContactFDir1 /*| dContactMu2*/ ) ;

        cp->setSoftERP(0.2) ;
        cp->setSoftCFM(1.0e-5) ;


        cp->setMu(0.5) ;
        cp->setMu2(0.75) ;




        // The world is very responsive
        space->setERP(0.9) ;
        space->setCFM(1.0e-5) ;
    }







    // the ground:
    {
        osg::ref_ptr<ODEObject> ground = dynamic_cast<ODEObject*>( osgDB::readObjectFile("large_ground.osgb") ) ;

        PS_ASSERT1( ground.valid() ) ;

        space->addObject(ground) ;

        // reset local collision parameters
        ground->asCollidable()->setCollisionParameters(NULL) ;
    }







    //
    // the car:
    //
    Car*    car = new Car() ;
    {
        car->init() ;

        space->addObject( car ) ;
    }








    // HUD
    //
    HUD*    hud = NULL ;

    if( true ) {
        hud = new HUD(car) ;

        hud->init( screen_aspect ) ;

        root->addChild( hud ) ;
    }










    //
    // The viewer. The pViewer package will not be released soon
    //

#if USE_PVIEWER

    pViewer::Viewer     viewer ;
    viewer.setEnableFullScreenEffects( ! NO_FX ) ;

    if( LOW ) {
        viewer.setRenderPass( pViewer::createLo(SKY_CUBE_PREFIX) ) ;
    } else {
        viewer.setRenderPass( pViewer::createHi(BLOOM, HDR, SKY_CUBE_PREFIX) ) ;
    }

#else

    osgViewer::Viewer   viewer ;


#endif



    viewer.setSceneData(root) ;









    //
    // the controller
    //
    {
        ControllerBase* controller = new ControllerBase() ;

        controller->onKeyPressed()->connect( car, &Car::handleKeyDown ) ;
        controller->onKeyReleased()->connect( car, &Car::handleKeyUp ) ;

        viewer.addEventHandler( controller ) ;
    }










    //
    // setup the camera manipulator:
    //
    {
        Transformable*  body = car->getBody() ;


        osgGA::CameraManipulator*   manipulator = new osgODEUtil::MatrixManipulator() ;


        body->setCameraManipulator( manipulator ) ;

        viewer.setCameraManipulator( manipulator ) ;




        // the eye is behind the car
        body->setCameraManipulatorCenter( osg::Vec3(0.0, -7.5, 1.0) ) ;

        // The camera looks toward the local Y axis
        body->setCameraManipulatorDirection( osg::Y_AXIS ) ;

        // The "up" axis is the global Z
        body->setCameraManipulatorUp( osg::Z_AXIS ) ;
        body->setCameraManipulatorUpObjectSpace( false ) ;

        // soft motion
        body->setCameraManipulatorElasticity( 10 ) ;
    }







    {
        osgViewer::StatsHandler*    stats = new osgViewer::StatsHandler() ;
        stats->setKeyEventTogglesOnScreenStats('0') ;

        viewer.addEventHandler(stats) ;
    }







    // Projection matrix

    viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, screen_aspect, 0.1, 1000.0) ;



    // enjoy

    viewer.run() ;



    return 0 ;
}




osg::Group*
create_light(bool nofx)
{
    osg::Group* group = NULL ;

    osg::Vec3   position = osg::Vec3(-1, 0, 0.5) ;
    position.normalize() ;


#if USE_PVIEWER

    if( ! nofx ) {
        group = new osg::Group() ;


        {
            pViewer::Light* light = new pViewer::Light() ;

            light->setPosition( osg::Vec4(position, 0.0) ) ;
            light->setDirection( osg::Vec4(-position, 0.0) ) ;
            light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0) ) ;
            light->setDiffuse( osg::Vec4(1.0, 0.99, 0.98, 1.0) ) ;
            light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

            light->setShadowEnabled(true) ;
            light->setShadowBufferSize(1024) ;
            light->setShadowBias(4.0) ;
            light->setShadowFrustumSize(15) ;

            light->setType( pViewer::Light::DIRECTIONAL ) ;

            group->addChild(light) ;
        }


        {
            pViewer::Light* light = new pViewer::Light() ;

            light->setAmbient( osg::Vec4(0.15, 0.16, 0.17, 1.0) ) ;

            light->setShadowEnabled(false) ;

            light->setType( pViewer::Light::AMBIENT ) ;

            group->addChild(light) ;
        }

    } else {

        osg::LightSource*   ls = new osg::LightSource() ;
        osg::Light* light = new osg::Light() ;

        light->setLightNum(0) ;
        light->setPosition( osg::Vec4(position, 0.0) ) ;
        light->setDirection( -position ) ;
        light->setAmbient( osg::Vec4(0.15, 0.16, 0.17, 1.0) ) ;
        light->setDiffuse( osg::Vec4(1.0, 0.99, 0.98, 1.0) ) ;
        light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

        ls->setLight(light) ;

        group = ls ;
    }



#else

    (void) nofx ;


    {
        osg::LightSource*   ls = new osg::LightSource() ;
        osg::Light* light = new osg::Light() ;

        light->setLightNum(0) ;
        light->setPosition( osg::Vec4(position, 0.0) ) ;
        light->setDirection( -position ) ;
        light->setAmbient( osg::Vec4(0.15, 0.16, 0.17, 1.0) ) ;
        light->setDiffuse( osg::Vec4(1.0, 0.99, 0.98, 1.0) ) ;
        light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) ) ;

        ls->setLight(light) ;

        group = ls ;
    }
#endif



    return group ;
}
