#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
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


    osg::Node*      cube = osgDB::readNodeFile("companioncube.osgb") ;

    PS_ASSERT1( cube != NULL ) ;

    /*
     * [1] Create the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setup(true, 1.0/60.0) ;

    /*
     * [2] Create the space
     */
    osgODE::Space*  space = new osgODE::Space() ;
    manager->setWorld( space ) ;


    /*
     * [3] Create a falling box
     */
    {
        // create the body and insert it into the world
        osgODE::Collidable* body = new osgODE::Box() ;
        manager->getWorld()->addObject( body ) ;

        body->setSize( osg::Vec3(1, 1, 1) ) ;

        // the mass
        body->setMass(512) ;


        // graphics
        body->getMatrixTransform()->addChild( cube ) ;


        // lift up the box
        body->setPosition( osg::Vec3(0.0, 0.0, 4.0) ) ;


        // let the box spin around the local Z axis...
        const double    W = 2.0 * osg::PI *  10 ;

        body->setAngularVelocity( osg::Z_AXIS * W ) ;
    }


    /*
     * [4] create a static cube
     */
    {
        // create the body
        osgODE::Collidable* body = new osgODE::Box() ;
        manager->getWorld()->addObject( body ) ;

        body->setSize( osg::Vec3(1, 1, 1) ) ;


        // graphics
        body->getMatrixTransform()->addChild( cube ) ;


        // make the body insensitive to the forces
        body->setKinematic(true) ;
    }


    /*
     * [5] save the result
     */
    osgDB::writeNodeFile(*manager, "output.osgb") ;


    return 0 ;
}
