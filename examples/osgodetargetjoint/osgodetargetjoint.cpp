#include <osgODE/Manager>
#include <osgODE/ManagerUpdateCallback>
#include <osgODE/RigidBody>
#include <osgODE/TargetJoint>
#include <osgODE/Notify>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    // graphics
    osg::Node*      r3_axis = osgDB::readNodeFile("axis3.osgb") ;

    PS_ASSERT1( r3_axis != NULL ) ;



    /*
     * [1] create and configure the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;


    manager->setup(  false,      // threaded
                     true,       // divert all visitors
                     1.0 / 60.0  // the step size
                 ) ;



    /*
     * [2] create the world
     */
    manager->setWorld( new osgODE::World() ) ;


    // no gravity
    manager->getWorld()->setGravity( osg::Vec3(0, 0, 0) ) ;


    /*
     * [3] create a body
     */
    osgODE::RigidBody*  body = new osgODE::RigidBody() ;
    {
        manager->getWorld()->addObject( body ) ;

        body->getMatrixTransform()->addChild( r3_axis ) ;
    }


    /*
     * [4] create the joint
     */
    {
        osgODE::TargetJoint*    joint = new osgODE::TargetJoint() ;

        manager->getWorld()->addObject( joint ) ;

        joint->setPosition( osg::Vec3(1, 0, 0) ) ;
        joint->setPositionTime( 10 ) ;

        joint->setQuaternion( osg::Quat(0.75 * osg::PI, osg::X_AXIS) * osg::Quat(0.75 * osg::PI, osg::Y_AXIS) * osg::Quat(0.75 * osg::PI, osg::Z_AXIS) ) ;
        joint->setQuaternionTime( 10 ) ;

        joint->setBody1( body ) ;

        joint->setParam( dParamERP, 1.0 ) ;
        joint->setParam( dParamCFM, 0.0 ) ;
    }


    /*
     * [5] write out the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
