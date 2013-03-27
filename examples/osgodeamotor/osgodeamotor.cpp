#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/AMotorJoint>
#include <osgODE/Notify>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;

    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;

    manager->setup(false, true, 1.0/60.0) ;

    manager->setWorld( new osgODE::World() ) ;

    manager->getWorld()->setGravity( osg::Vec3(0.0, 0.0, 0.0) ) ;




    osgODE::Joint*      motor = new osgODE::AMotorJoint() ;

    const double    FREQUENCY = 10.0 ;

    motor->setParam(dParamVel2, FREQUENCY * 2.0 * osg::PI) ;
    motor->setParam(dParamFMax2, 0.5) ;




    osg::Node*      axis = osgDB::readNodeFile("common/axis3.osgb") ;

    PS_ASSERT1( axis != NULL ) ;

    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*  b2 = new osgODE::RigidBody() ;

    b1->getMatrixTransform()->addChild(axis) ;
    b2->getMatrixTransform()->addChild(axis) ;

    b1->setPosition( osg::X_AXIS * -1.5 ) ;
    b2->setPosition( osg::X_AXIS *  1.5 ) ;




    motor->setBody1(b1) ;
    motor->setBody2(b2) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(motor) ;



    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
