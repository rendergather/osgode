#include <osgODE/Manager>
#include <osgODE/ManagerUpdateCallback>
#include <osgODE/RigidBody>
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


    // Ensure single threaded
    manager->setAutoStartThread( false ) ;

    // ManagerUpdateCallback::operator() calls the Manager::frame method
    manager->addUpdateCallback( new osgODE::ManagerUpdateCallback() ) ;

    // Divert the visitors
    manager->setAcceptVisitors( true ) ;

    // the step size
    manager->setStepSize( 1.0/60.0 ) ;


    // a shortcut for the 4 previous calls is:
    //
    // manager->setup(  false,      // threaded
    //                  true,       // divert all visitors
    //                  1.0 / 60.0  // the step size
    //              ) ;



    /*
     * [2] create the world
     */
    manager->setWorld( new osgODE::World() ) ;


    // no gravity
    manager->getWorld()->setGravity( osg::Vec3(0, 0, 0) ) ;


    /*
     * [3] create a body
     */
    {
        // create
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;

        manager->getWorld()->addObject( body ) ;


        // graphics
        body->getMatrixTransform()->addChild( r3_axis ) ;


        // Let the body spin
        const double    FREQUENCY = 1.0/4.0 ;

        body->setAngularVelocity( osg::Vec3(0.0, 0.0, FREQUENCY * 2.0 * osg::PI) ) ;
    }


    /*
     * [5] write out the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;


    return 0 ;
}
