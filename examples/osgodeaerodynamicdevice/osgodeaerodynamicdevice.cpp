#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/TriMesh>
#include <osgODE/AerodynamicDevice>
#include <osgODE/Notify>

#include <osgODEUtil/CreateTriMeshFromNode>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::Node*      cube = osgDB::readNodeFile("companioncube.osgt") ;

    PS_ASSERT1( cube != NULL ) ;




    /*
     * [1] Create the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;

    manager->setup(true, 1.0/100.0) ;

    manager->setWorld( new osgODE::Space() ) ;


    manager->getWorld()->setWind( osg::Vec3(-1.0, 0.0, 0.0) ) ;
    manager->getWorld()->setWindFrequency( 4.0 ) ;




    /*
     * [3] Create a kinematic plane as floor
     */
    {
        osg::Node*  plane = osgDB::readNodeFile( "floor.osgt" ) ;

        PS_ASSERT1( plane != NULL ) ;



        // build the motionless trimesh
        osgODE::TriMesh*    trim = osgODEUtil::createTriMeshFromNode(plane) ;

        trim->setKinematic(true) ;


        // add the trimesh to the space
        manager->getWorld()->addObject(trim) ;

        // add the graphic node to the simulation
        trim->getMatrixTransform()->addChild(plane) ;
    }




    /*
     * [4] Create a dynamic cube without an aerodynamic device
     */
    {
        osgODE::Collidable*     box = new osgODE::Box() ;
        box->setSize( osg::Vec3(1, 1, 1) ) ;

        // graphics
        box->getMatrixTransform()->addChild(cube) ;

        // move the cube to the left
        box->setPosition( osg::Vec3(-4, 0.0, 10.0) ) ;

        // insert the cube into the world
        manager->getWorld()->addObject(box) ;
    }




    /*
     * [5] Create a cube with an aerodynamic device
     */
    {
        osgODE::Collidable*     box = new osgODE::Box() ;
        box->setSize( osg::Vec3(1, 1, 1) ) ;

        // graphics
        box->getMatrixTransform()->addChild(cube) ;

        // move the cube to the right
        box->setPosition( osg::Vec3(4, 0.0, 10.0) ) ;

        // insert the cube into the world
        manager->getWorld()->addObject(box) ;



        // setup the aerodynamic device
        osgODE::AerodynamicDevice*  ad = new osgODE::AerodynamicDevice() ;

        // just a drag point in the front lower left vertex
        ad->addDragPoint( osg::Vec4(-0.5, -0.5, -0.5, 5.0) ) ;

        box->addUpdateCallback( ad ) ;
    }




    /*
     * [6] write out the graph
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;




    // bye
    return 0 ;
}
