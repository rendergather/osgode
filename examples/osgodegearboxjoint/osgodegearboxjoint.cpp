#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/GearboxJoint>
#include <osgODE/HingeJoint>
#include <osgODE/Notify>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    /*
     * [1] Create and setup the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setup(true, 1.0/60.0) ;

    manager->setWorld( new osgODE::World() ) ;



    /*
     * [2] create the joint
     */
    osgODE::GearboxJoint*   j = new osgODE::GearboxJoint() ;


    // also works with negative ratios
    j->setRatio(1.0/2.0) ;

//     j->setFriction(0.25) ;






    /*
     * [3] Creates two bodies
     */
    osg::Node*      weight = osgDB::readNodeFile("companioncube.osgt") ;

    PS_ASSERT1( weight != NULL ) ;


    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*  b2 = new osgODE::RigidBody() ;

    b1->getMatrixTransform()->addChild(weight) ;
    b2->getMatrixTransform()->addChild(weight) ;

    b1->setPosition( osg::X_AXIS * -1.0 ) ;
    b2->setPosition( osg::X_AXIS *  1.0 ) ;

    b1->setMass(1.0) ;
    b2->setMass(1.0) ;


    b1->setGravityMode(false) ;
    b2->setGravityMode(false) ;




    /*
     * [4] Constrain the bodies and add everything to the world
     */
    j->setBody1(b1) ;
    j->setBody2(b2) ;

    j->setAxis1(osg::Y_AXIS) ;
    j->setAxis2(osg::Y_AXIS) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(j) ;



    {
        osgODE::Joint*  hinge = new osgODE::HingeJoint() ;

        hinge->setBody1(b1) ;

        hinge->setAnchor1( b1->getPosition() ) ;
        hinge->setAxis1( osg::Y_AXIS ) ;

        const ooReal    FREQUENCY = 0.5 ;

        hinge->setParam(dParamVel, FREQUENCY * 2.0 * osg::PI) ;
        hinge->setParam(dParamFMax, 1.0) ;


        manager->getWorld()->addObject(hinge) ;
    }



    /*
     * [5] write the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
