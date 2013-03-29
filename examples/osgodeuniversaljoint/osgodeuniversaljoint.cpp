#include <osgODE/Manager>
#include <osgODE/RigidBody>
#include <osgODE/UniversalJoint>
#include <osgODE/HingeJoint>
#include <osgODE/Notify>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/export"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::Node*      arm_graphics   = osgDB::readNodeFile("universal_arm.osgb") ;
    osg::Node*      joint_graphics = osgDB::readNodeFile("universal_joint.osgb") ;

    PS_ASSERT1( arm_graphics != NULL ) ;
    PS_ASSERT1( joint_graphics != NULL ) ;



    // setup the scene
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    osgODE::RigidBody*              body1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*              body2 = new osgODE::RigidBody() ;
    osgODE::Joint*                  hinge1 = new osgODE::HingeJoint() ;
    osgODE::Joint*                  hinge2 = new osgODE::HingeJoint() ;
    osgODE::Joint*                  universal= new osgODE::UniversalJoint() ;


    {
        osgODE::World*  world = new osgODE::World() ;

        world->addObject(body1) ;
        world->addObject(body2) ;
        world->addObject(universal) ;
        world->addObject(hinge1) ;
        world->addObject(hinge2) ;


        manager->setWorld( world ) ;

        manager->setup(false, true, 1.0/100.0) ;

        world->setERP(1.0) ;
        world->setCFM(1.0e-5) ;

//         world->setGravity( osg::Vec3() ) ;
    }


    //
    // create the bodies
    //
    {
        body1->getMatrixTransform()->addChild(arm_graphics) ;

        body2->getMatrixTransform()->addChild(arm_graphics) ;
        body2->setQuaternion(  osg::Quat( osg::inDegrees(90.0), osg::Z_AXIS ) * osg::Quat( osg::inDegrees(-150.0), osg::Y_AXIS ) ) ;
    }



    //
    // create the universal
    //
    {
        universal->setBody1(body1) ;
        universal->setBody2(body2) ;

        universal->setAxis1( osg::X_AXIS ) ;
        universal->setAxis2( osg::Y_AXIS ) ;

        universal->getMatrixTransform()->addChild(joint_graphics) ;
    }



    //
    // create the first hinge
    //
    {
        hinge1->setBody1(body1) ;

        hinge1->setAxis1( osg::Z_AXIS ) ;
    }



    //
    // create the second hinge
    //
    {
        hinge2->setBody1(body2) ;

        hinge2->setAxis1( body2->getQuaternion() * -osg::Z_AXIS ) ;

        const double    FREQUENCY = 100.0 ;
        const double    FORCE = 1.0 ;

        hinge2->setParam( dParamVel1, FREQUENCY * 2.0 * osg::PI ) ;
        hinge2->setParam( dParamFMax1, FORCE ) ;
    }



    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
