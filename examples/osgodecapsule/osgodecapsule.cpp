#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/TriMesh>
#include <osgODE/Capsule>
#include <osgODE/DefaultNearCallback>
#include <osgODE/Notify>
#include <osgODE/CommonWorldOperations>

#include <osgODEUtil/CreateTriMeshFromNode>

#include <osgViewer/Viewer>

#include <osg/CullFace>
#include <osg/Math>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data"
#endif




#ifndef STEP_SIZE
    #define STEP_SIZE  (1.0/60.0)
#endif

#ifndef INT_TIME
    #define INT_TIME 0.5
#endif



// This class removes a body if it falls down below [0, 0, -1]
class RemoveObjectCallback: public osgODE::ODECallback
{
public:
    virtual void    operator()(osgODE::ODEObject* obj)
    {
        osgODE::RigidBody*  body = obj->asRigidBody() ;

        if( body->getPosition().z() < -1.0 ) {
            body->getWorld()->addOperation( new osgODE::RemoveObjectOperation(body) ) ;
        }
    }
} ;



// create a body every dt seconds
class AddCapsuleCallback: public osgODE::ODECallback
{
public:
    AddCapsuleCallback(float dt):
        m_dt(dt),
        m_start_time(0.0)
    {
        m_template = _createTemplate() ;
    }


    virtual void    operator()(osgODE::ODEObject* obj)
    {
        osgODE::World*  world = obj->asWorld() ;

        float   dt = world->getSimulationTime() - m_start_time ;

        if( dt >= m_dt ) {
            m_start_time = world->getSimulationTime() ;

            world->addOperation( new osgODE::AddObjectOperation(_createObject()) ) ;
        }
    }



private:
    float   _rand(void) { return (float)rand() / (float)RAND_MAX ; }



    osgODE::ODEObject*  _createObject(void)
    {
        osgODE::Collidable* collidable = static_cast<osgODE::Collidable*>( m_template->clone(osg::CopyOp::SHALLOW_COPY) ) ;

        ooReal  X = 2 * _rand() - 1 ;
        ooReal  Y = 2 * _rand() - 1 ;
        ooReal  Z = _rand() + 5 ;

        collidable->setPosition( osg::Vec3(X, Y, Z) ) ;


        return collidable ;
    }





    osgODE::Collidable* _createTemplate(void)
    {
        osg::Node*  graphics = osgDB::readNodeFile("capsule.osgt") ;
        PS_ASSERT1( graphics != NULL ) ;




        osgODE::Capsule*    capsule = new osgODE::Capsule() ;
        capsule->setSize( osg::Vec3(0.5, 0.5, 1.0) ) ;

        capsule->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::BACK) ) ;



        capsule->getMatrixTransform()->addChild(graphics) ;


        capsule->addUpdateCallback( new RemoveObjectCallback() ) ;

        capsule->setMass(15.0) ;


        return capsule ;
    }



private:
    float   m_dt ;
    float   m_start_time ;

    osg::ref_ptr<osgODE::Collidable> m_template ;
} ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;

    srand( time(NULL) ) ;


    // the root node
    osg::Group*     root = new osg::Group() ;


    /*
     * [1] create the manager
     */
    osgODE::Manager*    manager = new osgODE::Manager() ;
    {
        manager->setup(true, STEP_SIZE) ;

        // setup the space
        osgODE::Space*  space = new osgODE::Space() ;
        manager->setWorld( space ) ;

        space->setWorldStepFunction(dWorldQuickStep) ;


        // add the manager to the simulation
        root->addChild(manager) ;
    }



    /*
     * [2] Create the floor
     */
    {
        osg::Node*  floor_graphics = osgDB::readNodeFile("floor.osgt") ;
        PS_ASSERT1( floor_graphics != NULL ) ;

        floor_graphics->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::BACK) ) ;

        osgODE::TriMesh*    trimesh = osgODEUtil::createTriMeshFromNode(floor_graphics) ;

        // hold the trimesh
        trimesh->setKinematic(true) ;


        // graphics
        trimesh->getMatrixTransform()->addChild( floor_graphics ) ;



        manager->getWorld()->addObject(trimesh) ;
    }



    /*
     * [3] This callback will create a new cube every INT_TIME seconds
     */
    manager->getWorld()->addUpdateCallback( new AddCapsuleCallback(INT_TIME) ) ;



    osgViewer::Viewer   viewer ;
    viewer.setSceneData(root) ;
    viewer.realize() ;

    return viewer.run() ;
}
