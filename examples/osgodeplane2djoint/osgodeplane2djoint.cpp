#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/Plane2DJoint>
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
     * [1] Create the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;

    manager->setup(true, 1.0/100.0) ;

    manager->setWorld( new osgODE::World() ) ;




    /*
     * [2] Create a plane
     */
    osgODE::Transformable*  xform = new osgODE::Transformable() ;
    {
        osg::Node*  plane = osgDB::readNodeFile( "floor.osgb" ) ;

        PS_ASSERT1( plane != NULL ) ;

        // add the graphic node to the simulation
        xform->getMatrixTransform()->addChild(plane) ;

        // insert the plane into the world
        manager->getWorld()->addObject(xform) ;


        // rotate the plane
        osg::Matrix rotation = osg::Matrix::rotate( osg::Quat(osg::PI / 8.0, osg::Y_AXIS) ) * osg::Matrix::rotate( osg::Quat(osg::PI / 8.0, osg::X_AXIS) ) ;
        osg::Matrix position = osg::Matrix::translate( osg::Z_AXIS * 2.0 ) ;
        xform->getMatrixTransform()->setMatrix( rotation * position ) ;
    }




    /*
     * [3] Create a dynamic cube
     */

    osgODE::RigidBody*  dynamic_cube = new osgODE::RigidBody() ;
    {
        osg::Node*      cube = osgDB::readNodeFile("companioncube.osgb") ;

        PS_ASSERT1( cube != NULL ) ;

        // graphics
        dynamic_cube->getMatrixTransform()->addChild(cube) ;

        // insert the cube into the world
        manager->getWorld()->addObject(dynamic_cube) ;
    }




    /*
     * [3] Create the joint
     */
    {
        osgODE::Joint*  plane2d = new osgODE::Plane2DJoint() ;

        plane2d->setBody1( dynamic_cube ) ;


        manager->getWorld()->addObject( plane2d ) ;




        const osg::Matrix&  matrix = xform->getMatrixTransform()->getMatrix() ;

        plane2d->setAxis1( osg::Matrix::transform3x3( osg::Z_AXIS, matrix ) ) ;

        plane2d->setAnchor1( osg::Vec3(0,0,0) * matrix ) ;
    }




    /*
     * [5] write out the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;




    // bye
    return 0 ;
}
