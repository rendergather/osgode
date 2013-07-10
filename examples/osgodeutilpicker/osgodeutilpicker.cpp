#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/Box>
#include <osgODE/Notify>

#include <osgODEUtil/Picker>
#include <osgODEUtil/ControllerBase>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




class MyHandler: public osgODEUtil::ControllerBase
{
public:
    MyHandler( osgODEUtil::Picker* picker = NULL ):
    m_picker(picker)
    {
        onLeftMouseButton()->connect(this, &MyHandler::lmb) ;
        onMouseMoved()->connect(this, &MyHandler::moved) ;
    }


    void    lmb(const bool& pressed, bool& handled)
    {
//         handled = true ;


        if( ! pressed ) {

            osgODE::Collidable* picked = m_picker->pick( m_mouse_co ) ;

            if( picked ) {
                picked->setAngularVelocity( picked->getAngularVelocity() + osg::Z_AXIS * osg::PI ) ;
            }
        }
    }


    void    moved(const osg::Vec2& co, bool& handled)
    {
//         handled = true ;

        // normalized
        m_mouse_co = co ;
    }


private:
    osg::ref_ptr<osgODEUtil::Picker>    m_picker ;

    osg::Vec2   m_mouse_co ;
} ;




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



    // no gravity
    space->setGravity( osg::Vec3(0,0,0) ) ;



    /*
     * [3] Create some boxes
     */
    {
        // create the body and insert it into the world
        osg::ref_ptr<osgODE::Collidable>    body_tmpl = new osgODE::Box() ;

        body_tmpl->setSize( osg::Vec3(1, 1, 1) ) ;

        // the mass
        body_tmpl->setMass(512) ;


        // graphics
        body_tmpl->getMatrixTransform()->addChild( cube ) ;



        for(int r=-2; r<=2; r++) {
            for(int c=-2; c<=2; c++) {
                osgODE::RigidBody*  body = osg::clone( body_tmpl.get() ) ;

                body->setPosition( osg::Vec3(r*2, 0.0, c*2) ) ;

                space->addObject( body ) ;
            }
        }
    }





    /*
     * [4] Create the viewer and setup the picker
     */
    osgViewer::Viewer   viewer ;

    viewer.setSceneData( manager.get() ) ;

    viewer.realize() ;

    osgODEUtil::Picker* picker = new osgODEUtil::Picker() ;

    picker->setSpace( space ) ;
    picker->setCamera( viewer.getCamera() ) ;


    viewer.addEventHandler( new MyHandler( picker ) ) ;




    /*
     * [5] run
     */

//     viewer.setCameraManipulator( new osgGA::TrackballManipulator() ) ;

    return viewer.run() ;
}
