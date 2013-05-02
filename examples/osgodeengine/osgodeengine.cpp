#include <osgODE/Manager>
#include <osgODE/EngineBase>
#include <osgODE/RigidBody>
#include <osgODE/Notify>

#include <osgODEUtil/ControllerBase>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

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
    osg::ref_ptr< osgODE::EngineBase >          m_engine ;
    osg::ref_ptr< osgODE::RigidBody >           m_body ;
} ;




int
main(int argc, char** argv)
{
    osg::ArgumentParser argp(&argc, argv) ;

    if( argp.read("-h")  ||  argp.read("--help") ) {
        std::cout << "USAGE:"                                               << std::endl ;
        std::cout << "  v,V     decrease/increase vel"                      << std::endl ;
        std::cout << "  f,F     decrease/increase fmax"                     << std::endl ;
        std::cout << "  z       set fmax and vmax to 0"                     << std::endl ;
        std::cout << "  i,I     decrease/increase inertia"                  << std::endl ;
        std::cout << "  d,D     decrease/increase drag"                     << std::endl ;
        std::cout << "  n       set drag to 0 and inertia to 1"             << std::endl ;
        return 0 ;
    }


    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::ref_ptr<Scene> scene = new Scene() ;

    scene->init(argc, argv) ;


    return scene->mainloop() ;
}




void
Scene::init(int argc, char** argv)
{
    /*
     * [1] Create and setup the manager
     */
    m_manager = new osgODE::Manager() ;

    m_manager->setup(false, true, 1.0/100.0) ;

    m_manager->setWorld( new osgODE::World() ) ;



    /*
     * [2] create the engine
     */
    m_engine = new osgODE::EngineBase() ;






    /*
     * [2] Create a body for representing devices' speed
     */
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("axis3.osgb") ;
    PS_ASSERT1( node.valid() ) ;

    m_body = new osgODE::RigidBody() ;
    m_body->setKinematic( true ) ;

    m_body->getMatrixTransform()->addChild( node ) ;

    m_manager->getWorld()->addObject( m_body ) ;
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

    viewer.setCameraManipulator( new osgGA::TrackballManipulator() ) ;

    viewer.realize() ;

    double  last = 0.0 ;

    while( ! viewer.done() ) {

        double  cur_time = m_manager->getWorld()->getSimulationTime() ;
        double  dt = cur_time - last ;
        last = cur_time ;

        if( dt > 0.0 ) {
            double  t1, t2 ;
            t1 = 0 ;
            t2 = 0 ;
            m_engine->propagate( dt, t1, t2 ) ;
            m_engine->feedback( dt, 0 ) ;
        }

        m_body->setAngularVelocity( osg::Vec3(0, m_engine->getSpeed(), 0) ) ;

        viewer.frame() ;
    }


    return 1 ;
}




void
Scene::handleKeyDown(const int& key, const int& mod, bool& handled)
{


    if( key == 'v' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setVel( m_engine->getVel() - 0.25 * osg::PI ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'V' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setVel( m_engine->getVel() + 0.25 * osg::PI ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'f' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setFMax( m_engine->getFMax() * 0.5 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'F' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setFMax( m_engine->getFMax() * 2.0 ) ;

        if( m_engine->getFMax() == 0.0 ) {
            m_engine->setFMax( 1.0 ) ;
        }

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'z' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setFMax( 0.0 ) ;
        m_engine->setVel( 0.0 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'i' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setInertia( m_engine->getInertia() * 0.5 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'I' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setInertia( m_engine->getInertia() * 2.0 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'd' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setDrag( m_engine->getDrag() - 0.1 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'D' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setDrag( m_engine->getDrag() + 0.1 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'r' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setRatio( m_engine->getRatio() * 0.5 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'R' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setRatio( m_engine->getRatio() * 2.0 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }


    if( key == 'n' ) {
        m_manager->getWorld()->writeLock() ;

        m_engine->setDrag( 0.0 ) ;
        m_engine->setInertia( 1.0 ) ;

        m_manager->getWorld()->writeUnlock() ;

        handled = true ;
    }
}




void
Scene::handleKeyUp(const int& key, const int& mod, bool& handled)
{
}
