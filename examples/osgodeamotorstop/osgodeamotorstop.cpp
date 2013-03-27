#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/AMotorJoint>
#include <osgODE/BallJoint>
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


    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;

    manager->setup(false, true, 1.0/60.0) ;

    manager->setWorld( new osgODE::Space() ) ;

    manager->getWorld()->setGravity( osg::Vec3(0.0, 0.0, -1.0) ) ;





    osgODE::Box*            base = new osgODE::Box() ;
    {
        const osg::Vec3 size = osg::Vec3(10, 10, 1) ;

        base->setSize( size ) ;
        base->setPosition( osg::Vec3(0.0, 0.0, 0.0) ) ;
        base->setMass(1.0e4) ;

        osg::Node*  graphics = osgDB::readNodeFile("common/ground.osgb") ;
        PS_ASSERT1( graphics != NULL ) ;

        base->getMatrixTransform()->addChild( graphics ) ;
    }



    osg::Node*  weight_graphics = osgDB::readNodeFile("common/weight512.osgb") ;
    PS_ASSERT1( weight_graphics != NULL ) ;


    osgODE::Box*            weight1 = new osgODE::Box() ;
    {
        weight1->setSize( osg::Vec3(0.8, 0.8, 0.8) ) ;
        weight1->setPosition( osg::Vec3(4.0, -4.0, 0.9) ) ;
        weight1->setMass(512.0) ;

        weight1->getMatrixTransform()->addChild( weight_graphics ) ;
    }


    osgODE::Box*            weight2 = new osgODE::Box() ;
    {
        weight2->setSize( osg::Vec3(0.8, 0.8, 0.8) ) ;
        weight2->setPosition( osg::Vec3(4.0, 0.0, 0.9) ) ;
        weight2->setMass(512.0) ;

        weight2->getMatrixTransform()->addChild( weight_graphics ) ;
    }


    osgODE::Box*            weight3 = new osgODE::Box() ;
    {
        weight3->setSize( osg::Vec3(0.8, 0.8, 0.8) ) ;
        weight3->setPosition( osg::Vec3(4.0, 4.0, 0.9) ) ;
        weight3->setMass(512.0) ;

        weight3->getMatrixTransform()->addChild( weight_graphics ) ;
    }




    osgODE::BallJoint*  ball = new osgODE::BallJoint() ;
    {
        ball->setBody1(base) ;

        ball->setAnchor1( osg::Vec3(0.0, 0.0, 0.0) ) ;
    }




    osgODE::AMotorJoint*    motor = new osgODE::AMotorJoint() ;
    {

        motor->setBody1(base) ;


        motor->setAxis1(osg::Z_AXIS ) ;
        motor->setAxis3(osg::Y_AXIS ) ;



        motor->setMotorMode(dAMotorEuler) ;


        motor->setParam(dParamLoStop1, 0.0) ;
        motor->setParam(dParamHiStop1, 0.0) ;
        motor->setParam(dParamLoStop2, 0.0) ;
        motor->setParam(dParamHiStop2, 0.0) ;
        motor->setParam(dParamLoStop3, -osg::PI / 12.0) ;
        motor->setParam(dParamHiStop3, osg::PI / 12.0) ;


        motor->setAxis1Anchor( osgODE::AMotorJoint::BODY1 ) ;
        motor->setAxis3Anchor( osgODE::AMotorJoint::BODY1 ) ;
    }





    {
        manager->getWorld()->addObject( ball ) ;
        manager->getWorld()->addObject( motor ) ;
        manager->getWorld()->addObject( base ) ;
        manager->getWorld()->addObject( weight1 ) ;
        manager->getWorld()->addObject( weight2 ) ;
        manager->getWorld()->addObject( weight3 ) ;
    }



    osgDB::writeNodeFile(*manager, "output.osgb") ;

    return 0 ;
}
