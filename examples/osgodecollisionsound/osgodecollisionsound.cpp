#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/TriMesh>
#include <osgODE/Box>
#include <osgODE/DefaultNearCallback>
#include <osgODE/Notify>

#include <osgODEUtil/CreateTriMeshFromNode>
#include <osgODEUtil/AddRemoveObjectOperation>

#include <osgViewer/Viewer>

#include <osg/CullFace>
#include <osg/Math>

#include <pSound/Source>
#include <pSound/Buffer>
#include <pSound/ALState>

#include <osgDB/ReadFile>




#ifndef STEP_SIZE
    #define STEP_SIZE  (1.0/60.0)
#endif

#ifndef INT_TIME
    #define INT_TIME 2.5
#endif




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif



osg::ref_ptr<pSound::Buffer>  hit_sound ;




namespace {
class PlayVisitor: public osg::NodeVisitor
{
    public:
        PlayVisitor(double gain = 1.0): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), m_gain(gain) {}

        virtual void    apply(osg::Node& node)
        {
            pSound::Source*   source = dynamic_cast<pSound::Source*>(&node) ;

            if( source ) {

                if( ! source->isPlaying() || source->getParam(pSound::Source::GAIN) < m_gain ) {
                    source->setParam( pSound::Source::GAIN, m_gain ) ;
                    source->play() ;
                }

//                 source->play() ;
            }

            traverse(node) ;
        }

    private:
        double  m_gain ;
} ;



class SoundCollisionCallback: public osgODE::CollisionCallback
{
public:
    virtual void    operator()(osgODE::Collidable* owner, osgODE::Collidable* other, osgODE::NearCallback* near_callback)
    {
        osgODE::DefaultNearCallback*    cbk = near_callback->asDefaultNearCallback() ;

        osg::Vec3   p ;
        osg::Vec3   n ;

        const osgODE::DefaultNearCallback::CollisionResult& cr = cbk->getCollisionResult() ;

        for(unsigned int i=0; i<cr.getContacts().size(); i++) {
            p.x() += cr.getContacts()[i].x() ;
            p.y() += cr.getContacts()[i].y() ;
            p.z() += cr.getContacts()[i].z() ;
        }

        for(unsigned int i=0; i<cr.getNormals().size(); i++) {
            n.x() += cr.getNormals()[i].x() ;
            n.y() += cr.getNormals()[i].y() ;
            n.z() += cr.getNormals()[i].z() ;
        }

        p /= cr.getContacts().size() ;

        n.normalize() ;


        osgODE::RigidBody*  b1 = owner ;
        osgODE::RigidBody*  b2 = other ;

        osg::Vec3   v1 = b1->getPointVelocity( osg::Vec4(p, 0.0) ) ;
        osg::Vec3   v2 = b2->getPointVelocity( osg::Vec4(p, 0.0) ) ;

        osg::Vec3   vv = v1 - v2 ;


        double      v = vv * n ;

        double      m = b1->getMass() + b2->getMass() ;
        double      e = 0.5 * m * v * v ;


        if( e > 1.0 ) {
            osg::ref_ptr<osg::NodeVisitor>  nv = new PlayVisitor( osg::clampTo(e/50.0, 0.0, 1.0) ) ;
            owner->accept(*nv) ;
        }

        traverse(owner, other, near_callback) ;
    }
} ;

osg::ref_ptr<SoundCollisionCallback>    sound_cbk = new SoundCollisionCallback() ;



// This class removes a body if it falls down below [0, 0, -1]
class RemoveObjectCallback: public osgODE::ODECallback
{
public:
    virtual void    operator()(osgODE::ODEObject* obj)
    {
        osgODE::RigidBody*  body = obj->asRigidBody() ;

        if( body->getPosition().z() < -1.0 ) {
            body->getWorld()->addOperation( new osgODEUtil::RemoveObjectOperation(body) ) ;
        }
    }
} ;



// create a body in [0, 0, 5]
class AddCubeCallback: public osgODE::ODECallback
{
public:
    AddCubeCallback(float dt):
        m_dt(dt),
        m_start_time(0.0)
    {
        m_templates[0] = _createTemplate("common/woodenbox1.osgb") ;
        m_templates[1] = _createTemplate("common/woodenbox2.osgb") ;
        m_templates[2] = _createTemplate("common/woodenbox3.osgb") ;
    }


    virtual void    operator()(osgODE::ODEObject* obj)
    {
        osgODE::World*  world = obj->asWorld() ;

        float   dt = world->getSimulationTime() - m_start_time ;

        if( dt >= m_dt ) {
            m_start_time = world->getSimulationTime() ;

            world->addOperation( new osgODEUtil::AddObjectOperation(_createObject()) ) ;
        }
    }



private:
    float   _rand(void) { return (float)rand() / (float)RAND_MAX ; }


    osgODE::ODEObject*  _createObject(void)
    {
        const unsigned int  template_idx = rand() % 3 ;

        osgODE::Collidable* collidable = static_cast<osgODE::Collidable*>( m_templates[template_idx]->clone(osg::CopyOp::SHALLOW_COPY) ) ;
        collidable->setPosition( osg::Vec3(2 * _rand() - 1, 2 * _rand() - 1, 4 + 2 * _rand()) ) ;

        return collidable ;
    }


    osgODE::Collidable* _createTemplate(const std::string& graphics)
    {
        osg::Object*    woodenbox = osgDB::readObjectFile(graphics) ;
        PS_ASSERT1( woodenbox != NULL ) ;

        osgODE::Collidable* box = dynamic_cast<osgODE::Collidable*>( woodenbox ) ;
        PS_ASSERT1( box != NULL ) ;

        box->setDamping(0.01, 0.01) ;
        box->setAutoDisableLinearThreshold(0.1) ;
        box->setAutoDisableAngularThreshold(0.1) ;

        box->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::BACK) ) ;


        box->addUpdateCallback( new RemoveObjectCallback() ) ;

        box->setCollisionCallback( sound_cbk.get() ) ;

        pSound::Source*   source = new pSound::Source() ;

        source->setBuffer( hit_sound.get() ) ;

        box->getMatrixTransform()->addChild( source ) ;


        return box ;
    }



private:
    float   m_dt ;
    float   m_start_time ;

    osg::ref_ptr<osgODE::Collidable> m_templates[3] ;
} ;

}




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;


    pSound::ALState::instance()->setDistanceModel(pSound::ALState::EXPONENT_DISTANCE_CLAMPED) ;

    hit_sound = dynamic_cast<pSound::Buffer*>( osgDB::readObjectFile("hit.wav") ) ;


    PS_ASSERT1( hit_sound ) ;


    srand( time(NULL) ) ;


    // the root node
    osg::Group*     root = new osg::Group() ;


    /*
     * [1] create the manager
     */
    osgODE::Manager*    manager = new osgODE::Manager() ;
    {
        // The mainloop will run in a separate thread
        manager->setAutoStartThread(true) ;

        manager->setAcceptVisitors(true) ;

        // setup the space
        osgODE::Space*  space = new osgODE::Space() ;
        manager->setWorld( space ) ;

        manager->setStepSize(STEP_SIZE) ;


        // tuning
        space->setERP(0.2) ;
        space->setCFM(1.0e-5) ;

        space->setWorldStepFunction(dWorldQuickStep) ;

        osgODE::DefaultNearCallback*    ncbk = space->getNearCallback()->asDefaultNearCallback() ;
        ncbk->getCollisionParameters()->setMode( dContactApprox1 ) ;


        // add the manager to the simulation
        root->addChild(manager) ;
    }



    /*
     * [2] Create the floor
     */
    {
        osg::Node*  floor_graphics = osgDB::readNodeFile("common/ground.osgb") ;
        PS_ASSERT1( floor_graphics != NULL ) ;


        osgODE::TriMesh*    trimesh = osgODEUtil::createTriMeshFromNode(floor_graphics) ;

        // face culling
        trimesh->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::BACK) ) ;

        // hold the trimesh
        trimesh->setKinematic(true) ;


        // graphics
        trimesh->getMatrixTransform()->addChild( floor_graphics ) ;


        manager->getWorld()->addObject(trimesh) ;
    }



    /*
     * [3] This callback will create a new cube every INT_TIME seconds
     */
    manager->getWorld()->addUpdateCallback( new AddCubeCallback(INT_TIME) ) ;



    osgViewer::Viewer   viewer ;
    viewer.setSceneData(root) ;
    viewer.realize() ;

    return viewer.run() ;
}
