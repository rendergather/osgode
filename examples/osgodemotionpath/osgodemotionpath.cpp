#include <osgODE/Manager>
#include <osgODE/ManagerUpdateCallback>
#include <osgODE/RigidBody>
#include <osgODE/TargetJoint>
#include <osgODE/MotionPath>
#include <osgODE/CubicVec3Interpolator>
#include <osgODE/LinearVec3Interpolator>
#include <osgODE/LinearQuatInterpolator>
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


    // graphics
    osg::Node*      r3_axis = osgDB::readNodeFile("common/axis3.osgb") ;

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


    /*
     * [3] create a body
     */
    osgODE::RigidBody*  body = new osgODE::RigidBody() ;
    {
        manager->getWorld()->addObject( body ) ;


        // graphics
        body->getMatrixTransform()->addChild( r3_axis ) ;


        // gravity off
        body->setGravityMode( false ) ;
    }




    /*
     * [4] create the joint
     */
    osgODE::TargetJoint*    joint = new osgODE::TargetJoint() ;
    {

        manager->getWorld()->addObject( joint ) ;

        joint->setBody1( body ) ;

        joint->setParam( dParamERP, 1.0 ) ;
        joint->setParam( dParamCFM, 0.0 ) ;
    }




    /*
     * [5] create the motion path
     */
    osgODE::MotionPath* path = new osgODE::MotionPath() ;
    {
        joint->addUpdateCallback( path ) ;
        path->setStatus( osgODE::MotionPath::PLAY ) ;


        osgODE::Vec3Interpolator*   position = new osgODE::CubicVec3Interpolator() ;
        osgODE::QuatInterpolator*   quaternion = new osgODE::LinearQuatInterpolator() ;

        path->setPositionInterpolator( position ) ;
        path->setQuaternionInterpolator( quaternion ) ;


        position->setLooping(true) ;
        quaternion->setLooping(true) ;



#define REAL_RAND() (2.0 * (double) rand() / (double) RAND_MAX - 1.0)
#define VEC3_RAND() osg::Vec3(REAL_RAND(), REAL_RAND(), REAL_RAND())


        srand( time(NULL) ) ;

        osg::Vec3       point0 = VEC3_RAND() ;

        osg::Vec3   axis0 = VEC3_RAND() ;
        axis0.normalize() ;

        double  r0 = osg::PI * REAL_RAND() ;


        position->addPoint(0, point0) ;
        quaternion->addPoint(0, osg::Quat(r0, axis0)) ;

        for(int i=1; i<10; i++) {

            position->addPoint(i, VEC3_RAND()) ;


            double  r = osg::PI * REAL_RAND() ;
            osg::Vec3   axis = VEC3_RAND() ;
            axis.normalize() ;

            quaternion->addPoint(i * 2.5, osg::Quat(r, axis)) ;
        }

        position->addPoint(10, point0) ;
        quaternion->addPoint(25, osg::Quat(r0, axis0)) ;
    }





    /*
     * [6] write out the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;


    return 0 ;
}
