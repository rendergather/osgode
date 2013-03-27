#include <osgODE/BallJoint>
#include <osgODE/RigidBody>
#include <osgODE/World>
#include <osgODE/Manager>
#include <osgODE/CubicVec3Interpolator>
#include <osgODE/Notify>

#include <osg/LineWidth>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif




class RopeCallback: public osg::Drawable::UpdateCallback
{
public:
    RopeCallback(osgODE::RigidBody** bodies, unsigned int num_bodies):
    m_bodies(bodies),
    m_num_bodies(num_bodies)
    {
        m_interpolator = new osgODE::CubicVec3Interpolator() ;
    }





    virtual void    update(osg::NodeVisitor* nv, osg::Drawable* drawable)
    {
        osg::Geometry*  rope = drawable->asGeometry() ;

        PS_ASSERT1( rope ) ;


        _initInterpolator() ;
        _interpolate(rope, 0.01) ;
    }





private:




    void    _initInterpolator(void)
    {
        m_interpolator->clear() ;


        for(unsigned int i=0; i<m_num_bodies; i++) {
            double  t = (double)i / (double)m_num_bodies ;

            osg::Vec3   p = m_bodies[i]->getPosition() ;

            m_interpolator->addPoint(t, p) ;
        }
    }





    void    _interpolate(osg::Geometry* rope, double scale)
    {

        rope->setPrimitiveSetList( osg::Geometry::PrimitiveSetList() ) ;

        osg::Vec3Array* vertices = new osg::Vec3Array() ;

        for(double t=0.0; t<1.0 + scale * 0.5; t+= scale) {
            osg::Vec3   p = m_interpolator->interpolate(t) ;

            vertices->push_back(p) ;
        }


        rope->setVertexArray( vertices ) ;

        rope->addPrimitiveSet( new osg::DrawArrays(GL_LINE_STRIP, 0, vertices->size() ) ) ;
    }




    osg::ref_ptr<osgODE::Vec3Interpolator>  m_interpolator ;

    osgODE::RigidBody** m_bodies ;
    unsigned int        m_num_bodies ;
} ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    osg::Group*     root = new osg::Group() ;



    osgODE::Manager*    manager = new osgODE::Manager() ;
    osgODE::World*      world = new osgODE::World() ;

    manager->setWorld(world) ;

    manager->setup(false, true, 1.0/100.0) ;


    world->setERP(0.8) ;
    world->setCFM(1.0e-5) ;
    world->setWorldStepFunction( dWorldQuickStep ) ;



    const unsigned int  NUM_BODIES = 10 ;

    osgODE::RigidBody*  bodies[NUM_BODIES] ;

    for(unsigned int i=0; i<NUM_BODIES; i++) {
        osgODE::RigidBody*  body = new osgODE::RigidBody() ;
        osgODE::Joint*      ball = new osgODE::BallJoint() ;


        world->addObject(body) ;
        world->addObject(ball) ;


        body->setMass(0.1) ;
        body->setPosition( osg::Vec3(0, 0, i * 0.5) ) ;

        body->setDamping(1e-3, 1e-3) ;


        ball->setBody1(i != 0 ? bodies[i-1] : NULL) ;
        ball->setBody2(body) ;


        ball->setAnchor1( i != 0 ? bodies[i-1]->getPosition() : body->getPosition() ) ;
        ball->setAnchor2( ball->getAnchor1() ) ;


        bodies[i] = body ;
    }


    bodies[NUM_BODIES - 1]->setLinearVelocity( osg::X_AXIS * 1.0 ) ;
    bodies[NUM_BODIES - 1]->setMass( 8.0, osg::Vec3(0.4, 0.4, 0.4), dBoxClass ) ;

    osg::Node*  weight = osgDB::readNodeFile("common/weight8.osgb") ;
    PS_ASSERT1( weight ) ;

    bodies[NUM_BODIES - 1]->getMatrixTransform()->addChild( weight ) ;



    root->addChild(manager) ;


    osg::Geode* geode = new osg::Geode() ;
    root->addChild(geode) ;

    osg::Geometry*  rope = new osg::Geometry() ;
    geode->addDrawable(rope) ;

    rope->setSupportsDisplayList(false) ;
    rope->setDataVariance( osg::Object::DYNAMIC ) ;

    rope->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0) ) ;

    rope->setUpdateCallback( new RopeCallback(bodies, NUM_BODIES) ) ;



    osgGA::TrackballManipulator*    manipulator = new osgGA::TrackballManipulator() ;
    manipulator->setHomePosition( osg::Y_AXIS * -20, osg::Vec3(), osg::Z_AXIS ) ;


    osgViewer::Viewer   viewer ;

    viewer.setSceneData(root) ;
    viewer.setCameraManipulator( manipulator ) ;


    return viewer.run() ;
}
