#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/DifferentialJoint>
#include <osgODE/HingeJoint>
#include <osgODE/SliderJoint>
#include <osgODE/Notify>

#include <osgODEUtil/ControllerBase>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

#include <osg/ArgumentParser>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




class Scene: public osg::Referenced {
public:
    void    init(int argc, char** argv) ;
    int     mainloop(void) ;

    osg::Node*  getRoot() { return m_manager.get() ; }


    void    handleKeyDown(const int& key, const int& mod, bool& handled) ;
    void    handleKeyUp(const int& key, const int& mod, bool& handled) ;

private:
    osg::ref_ptr< osgODE::Manager >             m_manager ;
    osg::ref_ptr< osgODE::DifferentialJoint >   m_differential ;
    osg::ref_ptr< osgODE::Joint >               m_left_slider ;
    osg::ref_ptr< osgODE::Joint >               m_right_slider ;
} ;




int
main(int argc, char** argv)
{
    osg::ArgumentParser argp(&argc, argv) ;

    if( argp.read("-h")  ||  argp.read("--help") ) {
        std::cout << "USAGE:"                                               << std::endl ;
        std::cout << "  q,w     move left cube"                             << std::endl ;
        std::cout << "  e,r     move right cube"                            << std::endl ;
        std::cout << "  a,d     decrease/increase differential speed"       << std::endl ;
        std::cout << "  s       set differential speed to 0"                << std::endl ;
        std::cout << "  z,x     decrease/increase differential viscosity"   << std::endl ;
        std::cout << "  c,v     decrease/increase differential ratio"       << std::endl ;
        return 0 ;
    }


    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::ref_ptr<Scene> scene = new Scene() ;

    scene->init(argc, argv) ;



    /*
     * [5] write the graph
     */
//     osgDB::writeNodeFile( * scene->getRoot(), "output.osgb") ;

//     return 0 ;


    return scene->mainloop() ;
}




void
Scene::init(int argc, char** argv)
{
    /*
     * [1] Create and setup the manager
     */
    osg::ref_ptr<osgODE::Manager>   manager = new osgODE::Manager() ;
    manager->setup(true, 1.0/60.0) ;

    manager->setWorld( new osgODE::Space() ) ;



    /*
     * [2] create the joint
     */
    osgODE::DifferentialJoint*  j = new osgODE::DifferentialJoint() ;

    m_differential = j ;

    j->setAxis1(   osg::Y_AXIS ) ;
    j->setAxis2( - osg::Y_AXIS ) ;

    j->setParam( dParamFMax, 2.0 ) ;

    j->setParam( dParamCFM, 0.01 ) ;

    j->setFriction(0.0) ;






    /*
     * [3] Creates two bodies
     */
    osg::ref_ptr<osgODE::Collidable>    wheel = dynamic_cast<osgODE::Collidable*>( osgDB::readObjectFile("car_wheel.osgt") ) ;
    PS_ASSERT1( wheel.valid() ) ;

    // clear the local parameters, if there are any
    wheel->setCollisionParameters(NULL) ;

    // Just to be sure
    wheel->setAutoDisableFlag(false) ;


    wheel->setDamping(0, 0) ;


    wheel->setQuaternion( osg::Quat( 0.5 * osg::PI, osg::X_AXIS ) ) ;

    wheel->setGravityMode( false ) ;
    wheel->setMass( 1.0 ) ;

    osgODE::RigidBody*  b1 = osg::clone( wheel.get() ) ;
    osgODE::RigidBody*  b2 = osg::clone( wheel.get() ) ;


    b1->setPosition( osg::X_AXIS * -1.0 ) ;
    b2->setPosition( osg::X_AXIS *  1.0 ) ;




    /*
     * [4] Constrain the bodies and add everything to the world
     */
    j->setBody1(b1) ;
    j->setBody2(b2) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(b2) ;
    manager->getWorld()->addObject(j) ;


    m_manager = manager ;





    /*
     * [5] Constrain the rotation axis with an hinge
     */
    {
        osgODE::Joint*  hinge = new osgODE::HingeJoint() ;

        hinge->setAxis1( osg::Y_AXIS ) ;
        hinge->setBody1( b1 ) ;
        hinge->setAnchor1( b1->getPosition() ) ;

        manager->getWorld()->addObject(hinge) ;
    }

    {
        osgODE::Joint*  hinge = new osgODE::HingeJoint() ;

        hinge->setAxis1( osg::Y_AXIS ) ;
        hinge->setBody1( b2 ) ;
        hinge->setAnchor1( b2->getPosition() ) ;

        manager->getWorld()->addObject(hinge) ;
    }










    /*
     * Now create the bodies that act as brakes
     */

    {
        osg::ref_ptr<osgODE::Collidable>    cube = dynamic_cast<osgODE::Collidable*>( osgDB::readObjectFile( "woodenbox1.osgt" ) ) ;
        PS_ASSERT1( cube.valid() ) ;

        cube->setCollisionParameters(NULL) ;
        cube->setAutoDisableFlag(false) ;
        cube->setDamping(1.0e-2, 1.0e-2) ;

        cube->setSize( osg::Vec3(1,1,1) ) ;
        cube->setMass( 1.0 ) ;


        osgODE::RigidBody*  up_left = osg::clone( cube.get() ) ;
        osgODE::RigidBody*  up_right = osg::clone( cube.get() ) ;


        up_left->setPosition( osg::Vec3(-2.5, 0.0, 2.0) ) ;
        up_right->setPosition( osg::Vec3(2.5, 0.0, 2.0) ) ;


        osg::ref_ptr<osgODE::Joint> slider = new osgODE::SliderJoint() ;

        slider->setAxis1( osg::X_AXIS ) ;
        slider->setParam( dParamFMax,  10.0 ) ;


        m_left_slider = osg::clone( slider.get() ) ;
        m_right_slider = osg::clone( slider.get() ) ;

        m_left_slider->setBody1(up_left) ;
        m_right_slider->setBody1(up_right) ;


        manager->getWorld()->addObject( up_left ) ;
        manager->getWorld()->addObject( up_right ) ;
        manager->getWorld()->addObject( m_left_slider ) ;
        manager->getWorld()->addObject( m_right_slider ) ;




        osg::ref_ptr<osgODE::RigidBody> down_left = osg::clone( cube.get() ) ;
        osg::ref_ptr<osgODE::RigidBody> down_right = osg::clone( cube.get() ) ;


        down_left->setPosition( osg::Vec3(-2.5, 0.0, 0.0) ) ;
        down_right->setPosition( osg::Vec3(2.5, 0.0, 0.0) ) ;


        osg::ref_ptr<osgODE::Joint> hinge = new osgODE::HingeJoint() ;

        hinge->setAxis1( osg::Y_AXIS ) ;


        osgODE::Joint*  hinge_left = osg::clone( hinge.get() ) ;
        osgODE::Joint*  hinge_right = osg::clone( hinge.get() ) ;

        hinge_left->setBody1( up_left ) ;
        hinge_left->setBody2( down_left ) ;
        hinge_left->setAnchor1( up_left->getPosition() ) ;

        hinge_right->setBody1( up_right ) ;
        hinge_right->setBody2( down_right ) ;
        hinge_right->setAnchor1( up_right->getPosition() ) ;


        manager->getWorld()->addObject( down_left ) ;
        manager->getWorld()->addObject( down_right ) ;
        manager->getWorld()->addObject( hinge_left ) ;
        manager->getWorld()->addObject( hinge_right ) ;
    }
}




int
Scene::mainloop(void)
{
    osgViewer::Viewer   viewer ;

    viewer.setSceneData( m_manager.get() ) ;


    osgODEUtil::ControllerBase* controller = new osgODEUtil::ControllerBase() ;

    controller->onKeyPressed()->connect( this, &Scene::handleKeyDown ) ;
    controller->onKeyReleased()->connect( this, &Scene::handleKeyUp ) ;


    viewer.addEventHandler( controller ) ;


    return viewer.run() ;
}




void
Scene::handleKeyDown(const int& key, const int& mod, bool& handled)
{


    if( key == 'a' ) {
        m_differential->setParam( dParamVel, m_differential->getParam( dParamVel ) + osg::PI * 1.0 ) ;
    }


    if( key == 'd' ) {
        m_differential->setParam( dParamVel, m_differential->getParam( dParamVel ) - osg::PI * 1.0 ) ;
    }


    if( key == 's' ) {
        m_differential->setParam( dParamVel, 0.0 ) ;
    }


    if( key == 'q' ) {
        m_left_slider->setParam( dParamVel, -1.0 ) ;
    }


    if( key == 'w' ) {
        m_left_slider->setParam( dParamVel, 1.0 ) ;
    }


    if( key == 'e' ) {
        m_right_slider->setParam( dParamVel, -1.0 ) ;
    }


    if( key == 'r' ) {
        m_right_slider->setParam( dParamVel, 1.0 ) ;
    }


    if( key == 'x' ) {
        ooReal  friction = osg::clampTo(m_differential->getFriction() + 0.05, 0.0, 1.0) ;

        m_differential->setFriction( friction ) ;
    }


    if( key == 'z' ) {
        ooReal  friction = osg::clampTo(m_differential->getFriction() - 0.05, 0.0, 1.0) ;

        m_differential->setFriction( friction ) ;
    }


    if( key == 'c' ) {
        const ooReal    ratio = m_differential->getRatio1() * 0.5 ;

        m_differential->setRatio1( ratio ) ;
        m_differential->setRatio2( 1.0 / ratio ) ;
    }


    if( key == 'v' ) {
        const ooReal    ratio = m_differential->getRatio1() * 2.0 ;

        m_differential->setRatio1( ratio ) ;
        m_differential->setRatio2( 1.0 / ratio ) ;
    }


}




void
Scene::handleKeyUp(const int& key, const int& mod, bool& handled)
{


    if( key == 'q' ) {
        m_left_slider->setParam( dParamVel, 0.0 ) ;
    }


    if( key == 'w' ) {
        m_left_slider->setParam( dParamVel, 0.0 ) ;
    }


    if( key == 'e' ) {
        m_right_slider->setParam( dParamVel, 0.0 ) ;
    }


    if( key == 'r' ) {
        m_right_slider->setParam( dParamVel, 0.0 ) ;
    }
}
