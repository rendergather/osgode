#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/TriMesh>
#include <osgODE/Sphere>
#include <osgODE/DynamicParticleSystem>
#include <osgODE/DynamicParticleGeode>
#include <osgODE/CollisionPredictor>
#include <osgODE/HingeJoint>
#include <osgODE/Notify>

#include <osgODEUtil/CreateTriMeshFromNode>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>



#include <osgParticle/Particle>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#if 0
#include <pViewer/NodeMasks>
#endif




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




osg::MatrixTransform*   create_effect(osgODE::World* world, bool use_collision_prediction) ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;




    osg::ref_ptr<osg::Group>    root = new osg::Group() ;




    /*
     * [1] Create the manager and the space
     */
    osgODE::Manager*    manager = new osgODE::Manager() ;
    {
        root->addChild( manager ) ;

        manager->setup() ;

        manager->setWorld( new osgODE::Space() ) ;
    }




    /*
     * [2] Create the floor
     */
    {
        osg::Node*  plane = osgDB::readNodeFile( "floor.osgb" ) ;

        PS_ASSERT1( plane != NULL ) ;


        osg::MatrixTransform*   scale = new osg::MatrixTransform( osg::Matrix::scale( osg::Vec3(3,3,1) ) ) ;
        scale->addChild( plane ) ;

        scale->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON) ;



        // build the motionless trimesh
        osgODE::TriMesh*    trim = osgODEUtil::createTriMeshFromNode(scale) ;


        trim->setPosition( osg::Z_AXIS * -0.5 ) ;


        // add the trimesh to the space
        manager->getWorld()->addObject(trim) ;

        // add the graphic node to the simulation
        trim->getMatrixTransform()->addChild(scale) ;



        trim->setKinematic(true) ;
//         trim->setAngularVelocity( osg::Y_AXIS * 2.0 * osg::PI * -0.125 ) ;


#if 0
        plane->setNodeMask( pViewer::NodeMasks::ALBEDO |
                            pViewer::NodeMasks::MATERIAL |
                            pViewer::NodeMasks::NORMAL
        ) ;
#endif
    }




    /*
     * [3] Create the particle effects
     */
    {
        osg::MatrixTransform*   left = create_effect( manager->getWorld(), false ) ;
        osg::MatrixTransform*   right = create_effect( manager->getWorld(), true ) ;

        left->setMatrix( left->getMatrix() * osg::Matrix::translate( osg::X_AXIS * -7.5 ) ) ;
        right->setMatrix( right->getMatrix() * osg::Matrix::translate( osg::X_AXIS * 7.5 ) ) ;

        root->addChild( left ) ;
        root->addChild( right ) ;
    }


#if 0
    root->getOrCreateStateSet()->getOrCreateUniform("uMaterial", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(1.0, 0.8, 0.5, 1.0) ) ;
    root->getOrCreateStateSet()->getOrCreateUniform("uColor", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(0.8, 0.8, 0.8, 1.0) ) ;
#endif



    /*
     * [4] write out the graph
     */
    osgDB::writeNodeFile(*root, "output.osgb") ;




    // bye
    return 0 ;
}




osg::MatrixTransform*
create_effect(osgODE::World* world, bool use_collision_prediction)
{
    // create the particle system
    osgODE::DynamicParticleSystem   *ps = new  osgODE::DynamicParticleSystem() ;

    ps->setDefaultAttributes("", true, false) ;

    // set the world to use
    ps->setWorld( world ) ;


    //
    // Set the particle body template
    //
    osgODE::Collidable* body_template = new osgODE::Sphere() ;


    body_template->setSize( osg::Vec3(0.05, 0.05, 0.05) ) ;

    // tell the particles to not collide each other
    body_template->setCategoryBits(1) ;
    body_template->setCollideBits(~1) ;

    body_template->setMaxAngularSpeed( 0.0 ) ;
    body_template->setGyroscopicMode( false ) ;



    if( use_collision_prediction ) {
        body_template->addPostUpdateCallback( new osgODE::CollisionPredictor() ) ;
    }



    ps->setBodyTemplate( body_template ) ;



    // cretate the emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter() ;

    emitter->setParticleSystem(ps) ;



    osgParticle::RandomRateCounter *rrc =
        static_cast<osgParticle::RandomRateCounter *>( emitter->getCounter() ) ;

    rrc->setRateRange(20, 30);


    // the updater
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater() ;
    psu->addParticleSystem( ps ) ;



    // the particle template
    osgParticle::Particle   particle_template ;
    particle_template.setLifeTime(3) ;

    ps->setDefaultParticleTemplate( particle_template ) ;



    // The DynamicParticleGeode communicates the world-to-local
    // matrix to the particle systems. This is required because the
    // DynamicParticleSystem needs to transform bodies properties
    // from world frame to drawable local frame

    osg::Geode *geode = new osgODE::DynamicParticleGeode() ;
    geode->addDrawable(ps) ;


    osg::MatrixTransform*   mt = new osg::MatrixTransform() ;
    mt->addChild( geode ) ;
    mt->addChild( emitter ) ;
    mt->addChild( psu ) ;

    mt->setMatrix(  osg::Matrix::rotate( osg::PI, osg::Y_AXIS ) *
                    osg::Matrix::translate( osg::Z_AXIS * 25.0 )
        ) ;


#if 0
    geode->setNodeMask( pViewer::NodeMasks::TRANSPARENCY ) ;
#endif


    return mt ;
}
