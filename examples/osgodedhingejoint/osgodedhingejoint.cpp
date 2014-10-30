#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/DHingeJoint>
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
     *     The CFM will dumpen the rebound at the stop
     */
    osgODE::Joint*      j = new osgODE::DHingeJoint() ;






    /*
     * [3] Creates two bodies: a motionless one to the right side and a
     *     dynamic one to the left
     */
    osg::Node*      weight = osgDB::readNodeFile("companioncube.osgt") ;

    PS_ASSERT1( weight != NULL ) ;


    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;
    osgODE::RigidBody*  b2 = new osgODE::RigidBody() ;

    b1->getMatrixTransform()->addChild(weight) ;
    b2->getMatrixTransform()->addChild(weight) ;

    b1->setPosition( osg::X_AXIS * -5 ) ;
    b2->setPosition( osg::X_AXIS *  5 ) ;

    b1->setMass(1.0) ;
    b2->setMass(1.0) ;




    b1->setKinematic(true) ;




    /*
     * [4] Constrain the bodies and add everything to the world
     */
    j->setBody1(b1) ;
    j->setBody2(b2) ;

    j->setAnchor1( osg::X_AXIS * -2.5 ) ;
    j->setAnchor2( osg::X_AXIS *  2.5 ) ;

    osg::Vec3   axis = osg::Y_AXIS ;

    j->setAxis1(axis) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(j) ;



    /*
     * [5] store the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
