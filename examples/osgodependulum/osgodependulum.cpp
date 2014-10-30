#include <osgODE/Manager>
#include <osgODE/RigidBody>
#include <osgODE/HingeJoint>
#include <osgODE/Container>
#include <osgODE/Notify>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>




#define STEP_SIZE   5e-5
#define INITIAL_FORCE   osg::Vec3(0.1, 0.0, 0.0)




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;



    /*
     * [1] create the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setup(true, STEP_SIZE) ;


    /*
     * [2] create the world
     */
    manager->setWorld( new osgODE::World() ) ;



    osg::ref_ptr<osgODE::Container>     container = new osgODE::Container() ;



    // four bodies from top to the bottom

    osgODE::RigidBody*  dynamic_body1 = NULL ;
    osgODE::RigidBody*  dynamic_body2 = NULL ;
    osgODE::RigidBody*  dynamic_body3 = NULL ;

    // Lower body is not dynamic
    osgODE::RigidBody*  kinematic_body = NULL ;



    /*
     * [3] Create the upper body
     */
    {
        // create
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;
        container->addObject( body ) ;

        // no damping
        body->setLinearDamping( 0.0 ) ;
        body->setAngularDamping( 0.0 ) ;


        // graphics
        osg::Node*  node = osgDB::readNodeFile("pendulum_4.osgt") ;
        PS_ASSERT1( node ) ;


        body->getMatrixTransform()->addChild( node ) ;


        // translate the body
        body->setPosition( osg::Vec3(0.0, 0.0, 14.0) ) ;

        dynamic_body1 = body ;


        // tap the body to leave the equilibrium state
        body->addForce( INITIAL_FORCE ) ;
    }



    /*
     * [4] create the upper middle body
     */
    {
        // create
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;
        container->addObject( body ) ;

        // no damping
        body->setLinearDamping( 0.0 ) ;
        body->setAngularDamping( 0.0 ) ;


        // graphics
        osg::Node*  node = osgDB::readNodeFile("pendulum_3.osgt") ;
        PS_ASSERT1( node ) ;


        body->getMatrixTransform()->addChild( node ) ;


        // translate the body
        body->setPosition( osg::Vec3(0.0, 0.0, 12.0) ) ;

        dynamic_body2 = body ;
    }



    /*
     * [4] create the lower middle body
     */
    {
        // create
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;
        container->addObject( body ) ;

        // no damping
        body->setLinearDamping( 0.0 ) ;
        body->setAngularDamping( 0.0 ) ;


        // graphics
        osg::Node*  node = osgDB::readNodeFile("pendulum_2.osgt") ;
        PS_ASSERT1( node ) ;


        body->getMatrixTransform()->addChild( node ) ;


        // translate the body
        body->setPosition( osg::Vec3(0.0, 0.0, 8.0) ) ;

        dynamic_body3 = body ;
    }


    /*
     * [4] Create the kinematic lower body
     */
    {
        // create
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;
        container->addObject( body ) ;

        // no damping
        body->setLinearDamping( 0.0 ) ;
        body->setAngularDamping( 0.0 ) ;


        // graphics
        osg::Node*  node = osgDB::readNodeFile("pendulum_1.osgt") ;
        PS_ASSERT1( node ) ;


        body->getMatrixTransform()->addChild( node ) ;


        // hold the body
        body->setKinematic(true) ;

        kinematic_body = body ;
    }




    /*
     * [5] create the constraints
     */

    // the lower joint
    {
        // create
        osgODE::Joint*  joint = new osgODE::HingeJoint() ;
        container->addObject( joint ) ;


        // the bodies
        // We can attach the dynamic body to the static environment instead
        // of the kinematic body
        // Note that this causes a SIGSEGV with hinge2 joints

        // joint->setBody1(kinematic_body) ;
        joint->setBody2(dynamic_body3) ;


        // configure the joint
        joint->setAnchor1( kinematic_body->getPosition() ) ;
        joint->setAxis1( osg::Y_AXIS ) ;
    }


    // the middle joint
    {
        // create
        osgODE::Joint*  joint = new osgODE::HingeJoint() ;
        container->addObject( joint ) ;

        // the bodies
        joint->setBody1(dynamic_body3) ;
        joint->setBody2(dynamic_body2) ;

        // configure the joint
        joint->setAnchor1( dynamic_body3->getPosition() ) ;
        joint->setAxis1( osg::Y_AXIS ) ;
    }


    // the upper joint
    {
        // create
        osgODE::Joint*  joint = new osgODE::HingeJoint() ;
        container->addObject( joint ) ;

        // the bodies
        joint->setBody1(dynamic_body2) ;
        joint->setBody2(dynamic_body1) ;

        // configure the joint
        joint->setAnchor1( dynamic_body2->getPosition() ) ;
        joint->setAxis1( osg::Y_AXIS ) ;
    }





    // just testing the osgODE::Container copy constructor
    manager->getWorld()->addObject( osg::clone( container.get() ) ) ;



    /*
     * [6] write out the result
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;


    return 0 ;
}
