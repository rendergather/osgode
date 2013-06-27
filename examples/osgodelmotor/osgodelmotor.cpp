#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/LMotorJoint>
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


    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setup(true, 1.0/60.0) ;

    manager->setWorld( new osgODE::World() ) ;
    manager->getWorld()->setGravity( osg::Vec3(0.0, 0.0, 0.0) ) ;


    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*  b2 = new osgODE::RigidBody() ;

    osgODE::Joint*      j = new osgODE::LMotorJoint() ;


    osg::Node*      graphics = osgDB::readNodeFile("companioncube.osgb") ;
    PS_ASSERT1( graphics != NULL ) ;


    b1->getMatrixTransform()->addChild(graphics) ;
    b2->getMatrixTransform()->addChild(graphics) ;

    b1->setPosition( osg::X_AXIS * -1.0 + osg::Z_AXIS ) ;
    b2->setPosition( osg::X_AXIS *  1.0 - osg::Z_AXIS ) ;



    j->setBody1(b1) ;
    j->setBody2(b2) ;

    j->setParam(dParamVel, 1.0) ;
    j->setParam(dParamFMax, 0.1) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(j) ;


    osgDB::writeNodeFile(*manager, "output.osgb") ;


    return 0 ;
}
