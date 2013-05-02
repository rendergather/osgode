#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/TriMesh>
#include <osgODE/Character>
#include <osgODE/AMotorJoint>
#include <osgODE/LMotorJoint>
#include <osgODE/Notify>
#include <osgODE/DynamicParticleSystem>
#include <osgODE/DynamicParticleGeode>
#include <osgODE/Sphere>
#include <osgODE/Box>
#include <osgODE/CollisionPredictor>
#include <osgODE/AerodynamicDevice>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#include <osgODEUtil/CreateTriMeshFromNode>
#include <osgODEUtil/MatrixManipulator>
#include <osgODEUtil/ControllerBase>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "CharacterEx"



#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif





osg::Group* create_light(void) ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;



    osg::Group*     root = create_light() ;


//     psLogLevel = LOG_LEVEL_ALL ;




    /*
     * [1] Create the manager
     */
    osgODE::Manager*    manager = new osgODE::Manager() ;

    root->addChild( manager ) ;

    manager->setup(false, true, 1.0/120.0) ;

    manager->setWorld( new osgODE::Space() ) ;

    {
        osg::Vec3   wind( 1.0, 1.0, 0.0 ) ;
        wind.normalize() ;

        manager->getWorld()->setWind( wind * 40.0 / 3.6 ) ;
        manager->getWorld()->setWindFrequency( 8.0 ) ;
    }




    /*
     * [2] Create a kinematic floor
     */
    {
        osgODE::Collidable* ground = dynamic_cast<osgODE::Collidable*>( osgDB::readObjectFile("large_ground.osgb") ) ;
        PS_ASSERT1( ground != NULL ) ;

        manager->getWorld()->addObject(ground) ;

        // reset any local collision parameters
        ground->setCollisionParameters(NULL) ;
    }




    /*
     * [3] Create some cubes
     */
    {
        osg::ref_ptr<osgODE::Collidable>    cube_template = dynamic_cast<osgODE::Collidable*>( osgDB::readObjectFile("woodenbox1.osgb") ) ;
        PS_ASSERT1( cube_template.valid() ) ;

        cube_template->setDamping(1.0e-3, 1.0e-3) ;
        cube_template->setCollisionParameters(NULL) ;

        cube_template->setAutoDisableFlag( true ) ;
        cube_template->setAutoDisableTime( 3.0 ) ;
        cube_template->setAutoDisableSteps( 300 ) ;
        cube_template->setAutoDisableAverageSamplesCount( 10 ) ;
        cube_template->setAutoDisableLinearThreshold( 1.0e-2 ) ;
        cube_template->setAutoDisableAngularThreshold( osg::PI / 180.0 ) ;

        for( int r=-4; r<=4; r++ ) {

            for( int c=-4; c<=4; c++ ) {

                osgODE::Collidable*     coll = osg::clone( cube_template.get() ) ;

                coll->setPosition( osg::Vec3(r*10, c*10, 5.0) ) ;

                manager->getWorld()->addObject(coll) ;
            }
        }
    }




    /*
     * [4] Create the character
     */
    osgODE::CharacterEx*    character = new osgODE::CharacterEx() ;
    {

        character->init() ;

        character->getBody()->setPosition( osg::Z_AXIS * 10 + osg::Y_AXIS * -15 ) ;

        manager->getWorld()->addObject( character ) ;
    }




    /*
     * [5] setup the view
     */

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer() ;


    //
    // the controller
    //
    osgODEUtil::ControllerBase* controller = new osgODEUtil::ControllerBase() ;
    {
        viewer->addEventHandler( controller ) ;


        controller->onKeyPressed()          ->connect( character, &osgODE::CharacterEx::handleKeyDown       ) ;
        controller->onKeyReleased()         ->connect( character, &osgODE::CharacterEx::handleKeyUp         ) ;
        controller->onMouseMoved()          ->connect( character, &osgODE::CharacterEx::handleMove          ) ;
        controller->onLeftMouseButton()     ->connect( character, &osgODE::CharacterEx::handleLeftButton    ) ;
        controller->onRightMouseButton()    ->connect( character, &osgODE::CharacterEx::handleRightButton   ) ;
        controller->onFrame()               ->connect( character, &osgODE::CharacterEx::handleFrame         ) ;


        character->setSensitivity( 0.125 ) ;
    }




    {
        // create the particle system
        osgODE::DynamicParticleSystem   *ps = new  osgODE::DynamicParticleSystem() ;

        const char* texture = argc > 1 ? argv[1] : "" ;

        ps->setDefaultAttributes(texture, true, false) ;

        // set the world to use
        ps->setWorld( manager->getWorld() ) ;


        //
        // Set the particle body template
        //
        osgODE::Collidable* body_template = new osgODE::Sphere() ;

        body_template->setSize( osg::Vec3(0.2, 0.2, 0.2) ) ;

        // tell the particles to not collide each other
        body_template->setCategoryBits( 1 << 31 ) ;
        body_template->setCollideBits( ~ (1 << 31) ) ;

        body_template->setMaxAngularSpeed( 0.0 ) ;
        body_template->setGyroscopicMode( false ) ;

        body_template->setDensity( 1000.0 ) ;

        body_template->addPostUpdateCallback( new osgODE::CollisionPredictor() ) ;

        ps->setBodyTemplate( body_template ) ;

        osgODE::AerodynamicDevice*  ad = new osgODE::AerodynamicDevice() ;
        ad->addDragPoint( osg::Vec4(0.0, 0.0, 0.0, 3.0) ) ;

        body_template->addUpdateCallback( ad ) ;



        // cretate the emitter
        osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter() ;

        emitter->setParticleSystem(ps) ;



        osgParticle::RandomRateCounter *rrc =
            static_cast<osgParticle::RandomRateCounter *>( emitter->getCounter() ) ;

//         rrc->setRateRange(25, 25) ;
        rrc->setRateRange(50, 50) ;
//         rrc->setRateRange(100, 100) ;
//         rrc->setRateRange(200, 200) ;


        osgParticle::RadialShooter* shooter = new osgParticle::RadialShooter() ;
        emitter->setShooter( shooter ) ;

        shooter->setThetaRange( 0.0, osg::PI * 2.0 ) ;
        shooter->setPhiRange( 0.0, osg::PI * 2.0 ) ;
        shooter->setInitialSpeedRange( 10, 100 ) ;


        // the updater
        osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater() ;
        psu->addParticleSystem( ps ) ;



        // the particle template
        osgParticle::Particle   particle_template ;
        particle_template.setLifeTime(5) ;

        ps->setDefaultParticleTemplate( particle_template ) ;



        // The DynamicParticleGeode communicates the world-to-local
        // matrix to the particle systems. This is required because the
        // DynamicParticleSystem needs to transform bodies properties
        // from world frame to drawable local frame

        osg::Geode *geode = new osgODE::DynamicParticleGeode() ;
        geode->addDrawable(ps) ;

        osg::MatrixTransform*   effect = new osg::MatrixTransform() ;

        effect->addChild( geode ) ;
        effect->addChild( emitter ) ;
        effect->addChild( psu ) ;

        osg::Node*  companion = osgDB::readNodeFile("companioncube.osgb") ;
        PS_ASSERT1( companion != NULL ) ;


        osgODE::Collidable*     cube = new osgODE::Box() ;

        cube->getMatrixTransform()->addChild( companion ) ;

        // this collidable does not collide with the particles
        cube->setCategoryBits( 1 << 31 ) ;
        cube->setCollideBits( ~ (1 << 31) ) ;

        cube->setDamping(1.0e-3, 1.0e-3) ;
        cube->setCollisionParameters(NULL) ;

        cube->setAutoDisableFlag( false ) ;

        cube->setPosition( osg::Vec3(-45.0, 0.0, 5.0) ) ;

        cube->setSize( osg::Vec3(1,1,1) ) ;
        cube->setMass( 10.0 ) ;

        cube->getMatrixTransform()->addChild( effect ) ;

        manager->getWorld()->addObject( cube ) ;
    }



    character->getBody()->setCameraManipulator( new osgODEUtil::MatrixManipulator() ) ;
    character->getBody()->setCameraManipulatorCenter( osg::Vec3(0.0, 0.0, 0.0) ) ;
    character->getBody()->setCameraManipulatorDirection( -osg::Z_AXIS) ;
    character->getBody()->setCameraManipulatorUp( osg::Y_AXIS ) ;
    character->getBody()->setCameraManipulatorUpObjectSpace( true ) ;
    character->getBody()->setCameraManipulatorElasticity( 0 ) ;

    viewer->setCameraManipulator( character->getBody()->getCameraManipulator() ) ;



    osgViewer::StatsHandler*    stats = new osgViewer::StatsHandler() ;
    stats->setKeyEventTogglesOnScreenStats('0') ;

    viewer->addEventHandler(stats) ;

    viewer->addEventHandler( new osgViewer::ThreadingHandler() ) ;



    viewer->setSceneData( root ) ;

    viewer->run() ;


    viewer = NULL ;

    return 0 ;
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
