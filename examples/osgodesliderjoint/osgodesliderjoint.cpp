#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/SliderJoint>
#include <osgODE/Notify>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;



    /*
     * [1] Create and configure manager and world
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setAcceptVisitors(true) ;
    manager->setAutoStartThread(true) ;
    manager->setStepSize(1.0/60.0) ;

    manager->setWorld( new osgODE::World() ) ;



    /*
     * [2] create the constraint
     */
    osgODE::Joint*      j = new osgODE::SliderJoint() ;

    // The sliding axis
    j->setAxis1(osg::Z_AXIS) ;

    // Make the stop soft
    j->setParam(dParamLoStop, 0.0) ;
    j->setParam(dParamHiStop, 0.0) ;
    j->setParam(dParamStopCFM, manager->getStepSize() * 30.0) ;
    j->setParam(dParamStopERP, manager->getStepSize() * 20.0) ;






    /*
     * [3] Create two bodies: a kinematic one to the right and a dynamic one
     *     to the left
     */
    osg::Node*      graphics = osgDB::readNodeFile("common/companioncube.osgb") ;
    PS_ASSERT1( graphics != NULL ) ;


    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*  b2 = new osgODE::RigidBody() ;

    b1->getMatrixTransform()->addChild(graphics) ;
    b2->getMatrixTransform()->addChild(graphics) ;

    b1->setPosition( osg::X_AXIS * -1.5 ) ;
    b2->setPosition( osg::X_AXIS *  1.5 ) ;

    b1->setKinematic(true) ;




    /*
     * [4] set the bodies and add everything to the simulation
     */
    j->setBody1(b1) ;
    j->setBody2(b2) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(j) ;



    /*
     * [5] save the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
